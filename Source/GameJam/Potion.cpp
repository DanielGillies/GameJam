// Fill out your copyright notice in the Description page of Project Settings.

#include "Potion.h"
#include "PaperSpriteComponent.h"
#include "GameplayAbility.h"
#include "GameJamCharacter.h"
#include "PotionGameplayAbility.h"


// Sets default values
APotion::APotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();

	// Setup pickup handler
	Sprite->OnComponentBeginOverlap.AddDynamic(this, &APotion::OnOverlapBegin);

}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APotion::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this))
	{
		if (OtherActor->ActorHasTag(FName("Character.Hero")))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *SelectRandomAbility()->StaticClass()->GetName());
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *SelectRandomEffect()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("------"));
			//UE_LOG(LogTemp, Warning, TEXT("LOLOLOL"));
		}
	}
}

TSubclassOf<UPotionGameplayAbility> APotion::SelectRandomAbility()
{
	int32 index = FMath::RandRange(0, PossibleAbilities.Num() - 1);
	//PossibleAbilities[index]
	UPotionGameplayAbility* Ability = Cast<UPotionGameplayAbility>(PossibleAbilities[index]);
	if (Ability)
	{
		UE_LOG(LogTemp, Warning, TEXT("LOLOLOL"));
	}
	/*UGameplayEffect* Effect = Cast<UGameplayEffect>(SelectRandomEffect());
	Ability->SetEffectFromPotion(Effect);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Ability->GetEffectFromPotion()->GetName());*/
	return PossibleAbilities[index];
}

TSubclassOf<class UGameplayEffect> APotion::SelectRandomEffect()
{
	int32 index = FMath::RandRange(0, PossibleEffects.Num() - 1);
	return PossibleEffects[index];
}
