// Fill out your copyright notice in the Description page of Project Settings.

#include "AlchemyMovementComponent.h"
#include "GameJamCharacter.h"



float UAlchemyMovementComponent::GetMaxSpeed() const
{
	const AGameJamCharacter* Char = CastChecked<AGameJamCharacter>(GetOwner(), ECastCheckedType::NullAllowed);
	if (Char)
	{
		return Char->GetMaxSpeed();
	}

	return Super::GetMaxSpeed();
}
