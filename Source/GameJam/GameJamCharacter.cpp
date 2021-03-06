// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GameJamCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "AlchemyAttributeSet.h"
#include "AlchemyMovementComponent.h"
#include "PotionGameplayAbility.h"
#include "TimerManager.h"
#include "UObjectGlobals.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// AGameJamCharacter

AGameJamCharacter::AGameJamCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UAlchemyMovementComponent>(ACharacter::CharacterMovementComponentName)
	)
/*
AGASCharacter::AGASCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer
.SetDefaultSubobjectClass<UGASCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
)
{*/
{
	
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Ability system component
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	//GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
	//UDataTable* test = CreateDefaultSubobject<UDataTable>(FName("test"));
	AttributeSet = CreateDefaultSubobject<UAlchemyAttributeSet>(FName("Attributes"));
	//if (Attrs)
	//{
	//}
	//AttributeSet = AbilitySystem->InitStats(AttributeSet, AbilitySystem);

}

bool AGameJamCharacter::AddAbilityToArray(UPotionGameplayAbility* Ability)
{
	int32 AbilityIndex = -1;
	for (int i = 0; i < AbilityArray.Num(); i++)
	{
		if (!AbilityArray[i])
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND EMPTY-"));
			AbilityIndex = i;
			break;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), AbilityArray.Num());
	if (AbilityIndex != -1)
	{
		AbilityArray[AbilityIndex] = Ability;
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 1, AbilityIndex + StartingAbilities.Num()));
		UE_LOG(LogTemp, Warning, TEXT("Added ability %d : key %d"), AbilityIndex + 1, AbilityIndex + StartingAbilities.Num());
		return true;
	}
	else if (AbilityArray.Num() < 4)
	{
		AbilityIndex = AbilityArray.Add(Ability);
		AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability, 1, AbilityIndex + StartingAbilities.Num()));
		AbilitySystem->DecrementAbilityListLock()
		UE_LOG(LogTemp, Warning, TEXT("Added ability %d : key %d"), AbilityIndex + 1, AbilityIndex + StartingAbilities.Num());
		return true;
	}
	else
	{
		return false;
	}
}

void AGameJamCharacter::RemoveAbilityFromArray(UPotionGameplayAbility* Ability)
{
	int32 AbilityIndex = -1;
	for (int i = 0; i < AbilityArray.Num(); i++)
	{
		if (AbilityArray[i] == Ability)
		{
			AbilityIndex = i;
			break;
		}
	}
	if (AbilityIndex != -1)
	{
		AbilityArray[AbilityIndex] = nullptr;
		//AbilityArray.RemoveAt(AbilityIndex, 1, false);
		AbilitySystem->ClearAbility(Ability->GetCurrentAbilitySpecHandle());
		UE_LOG(LogTemp, Warning, TEXT("Removed ability %d"), AbilityIndex+1);
		UE_LOG(LogTemp, Warning, TEXT("Num abilities %d"), AbilityArray.Num());

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find the ability you wanted to remove."));
	}
}

void AGameJamCharacter::RemoveRandomAbility()
{
	int32 index = FMath::RandRange(0, AbilityArray.Num()-1);
	RemoveAbilityFromArray(AbilityArray[1]);
}

void AGameJamCharacter::PlayAttackAnimation()
{
	if (!AttackAnimationLock)
	{
		FTimerHandle AnimationLockHandle;
		AttackAnimationLock = true;
		UPaperFlipbook* DesiredAnimation = AttackAnimation;
		GetSprite()->SetFlipbook(DesiredAnimation);

		GetWorldTimerManager().SetTimer(AnimationLockHandle, this, &AGameJamCharacter::FinishAttackAnimation, GetSprite()->GetFlipbookLength());
	}
}

void AGameJamCharacter::FinishAttackAnimation()
{
	AttackAnimationLock = false;
}

FVector2D AGameJamCharacter::GetRelativeLocationToPosition(FVector CheckAgainst)
{
	FVector MyLocation = GetActorLocation();
	FVector OtherLocation = CheckAgainst;

	FVector2D MyLocation2D = FVector2D(MyLocation.X, MyLocation.Z);
	FVector2D OtherLocation2D = FVector2D(OtherLocation.X, OtherLocation.Z);

	FVector2D RelativeLocation = OtherLocation2D - MyLocation2D;

	return RelativeLocation;
}

void AGameJamCharacter::MoveAI(float Amount)
{
	MoveRight(Amount);
}

bool AGameJamCharacter::checkValidAbility(int index)
{
	if (index < AbilityArray.Num())
	{
		if (AbilityArray[index])
			return true;
	}
	return false;

}

//////////////////////////////////////////////////////////////////////////
// Animation

void AGameJamCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			for (int i = 0; i < StartingAbilities.Num(); i++)
			{
				TSubclassOf<UGameplayAbility> Ability = StartingAbilities[i];
				//UGameplayAbility* AbilityObject = Cast<UGameplayAbility>(Ability->GetDefaultObject());
					//NewObject<UGameplayAbility>(GetTransientPackage(), Ability, FName("Ability"));
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, i));
			}
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
		const UAttributeSet * Attrs = AbilitySystem->InitStats(UAlchemyAttributeSet::StaticClass(), nullptr);

		if (Attrs)
		{
			const UAlchemyAttributeSet* temp = Cast<UAlchemyAttributeSet>(Attrs);
			AttributeSet = const_cast<UAlchemyAttributeSet*>(temp);
			UE_LOG(LogTemp, Warning, TEXT("%s --> %f"), *GetName(), AttributeSet->MoveSpeed);
		}
	}

}

void AGameJamCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void AGameJamCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGameJamCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAction("TestInput", IE_Released, this, &AGameJamCharacter::RemoveRandomAbility);

	PlayerInputComponent->BindAxis("MoveRight", this, &AGameJamCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGameJamCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGameJamCharacter::TouchStopped);

	AbilitySystem->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));
}

void AGameJamCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void AGameJamCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void AGameJamCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void AGameJamCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	//UE_LOG(LogTemp, Warning, TEXT("LOCKED BITCH"));
	if (!AttackAnimationLock)
	{
		UpdateAnimation();
	}

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}
