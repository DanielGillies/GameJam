// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AlchemyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM_API UAlchemyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	//Hitpoints
	UPROPERTY(Category = "Character Attributes | Health", EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(Category = "Character Attributes | MaxHealth", EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(Category = "Character Attributes | MoveSpeed", EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(Category = "Character Attributes | Damage", EditAnywhere, BlueprintReadWrite)
	float Damage;
	
	
};
