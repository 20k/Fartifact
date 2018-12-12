// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FartifactGameMode.h"
#include "FartifactPlayerController.h"
#include "FartifactCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "FartifactGameStateBase.h"
#include "CardManager.h"

AFartifactGameMode::AFartifactGameMode()
{
	GameStateClass = AFartifactGameStateBase::StaticClass();

	// use our custom PlayerController class
	PlayerControllerClass = AFartifactPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AFartifactGameMode::BeginPlay()
{
	FCardManager test_cards;

	FCard tcard1;
	FCard tcard2;
	FCard tcard3;

	tcard1.which = (int)FCard::type::AMERICAN_DOLLAR;
	tcard2.which = (int)FCard::type::PESO;
	tcard3.which = (int)FCard::type::EURO;

	test_cards.Add({ tcard1, tcard2, tcard3 });
}