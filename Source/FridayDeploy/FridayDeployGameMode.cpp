// Copyright Epic Games, Inc. All Rights Reserved.

#include "FridayDeployGameMode.h"
#include "FridayDeployCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFridayDeployGameMode::AFridayDeployGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
