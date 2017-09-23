// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PotionGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM_API UPotionGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPotionGameplayAbility();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effect)
	class UGameplayEffect* EffectFromPotion;
	
};
