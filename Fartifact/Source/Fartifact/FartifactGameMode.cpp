// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FartifactGameMode.h"
#include "FartifactPlayerController.h"
#include "FartifactCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFartifactGameMode::AFartifactGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFartifactPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}