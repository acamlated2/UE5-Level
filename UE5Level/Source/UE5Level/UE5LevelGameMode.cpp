// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5LevelGameMode.h"
#include "UE5LevelCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5LevelGameMode::AUE5LevelGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
