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
	CardManager test_cards;

	Card tcard1;
	Card tcard2;
	Card tcard3;

	tcard1.which = Card::type::AMERICAN_DOLLAR;
	tcard2.which = Card::type::PESO;
	tcard3.which = Card::type::EURO;

	test_cards.Add({ tcard1, tcard2, tcard3 });
}