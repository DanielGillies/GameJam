// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GameJamGameMode.h"
#include "GameJamCharacter.h"

AGameJamGameMode::AGameJamGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGameJamCharacter::StaticClass();	
}
