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
	Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();

	if (PossibleAbilities.Num() < 1 || PossibleEffects.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Make sure you populate the potion's possible abilities and possible effects!"));
	}

}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UPotionGameplayAbility* APotion::SelectRandomAbility()
{
	int32 index = FMath::RandRange(0, PossibleAbilities.Num() - 1);
	//PossibleAbilities[index]
	UGameplayAbility* test = Cast<UGameplayAbility>(PossibleAbilities[index]);
	UPotionGameplayAbility* Ability = Cast<UPotionGameplayAbility>(test);

	if (Ability)
	{
		UGameplayEffect* Effect = Cast<UGameplayEffect>(SelectRandomEffect().GetDefaultObject());
		if (Effect)
		{
			Ability->EffectFromPotion = Effect;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Ability->EffectFromPotion->GetName());
		}
	}
	
	return Ability;
}

TSubclassOf<class UGameplayEffect> APotion::SelectRandomEffect()
{
	int32 index = FMath::RandRange(0, PossibleEffects.Num() - 1);
	return PossibleEffects[index];
}

