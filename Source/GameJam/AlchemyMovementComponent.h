// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AlchemyMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM_API UAlchemyMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	virtual float GetMaxSpeed() const override;
	
	
};
