// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FartifactGameMode.h"
#include "FartifactPlayerController.h"
#include "FartifactCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "FartifactGameStateBase.h"
#include "CardManager.h"
#include "FartifactPlayerController.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"

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
	FCardManager test_deck;

	FCard tcard1;
	FCard tcard2;
	FCard tcard3;

	tcard1.which = (int)FCard::type::AMERICAN_DOLLAR;
	tcard2.which = (int)FCard::type::PESO;
	tcard3.which = (int)FCard::type::EURO;

	test_deck.Add({ tcard1, tcard2, tcard3 });

	test_board_state.AddPlayerAndDeck(0, test_deck);
	test_board_state.AddPlayerAndDeck(1, test_deck);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *test_board_state.Debug());
	UE_LOG(LogTemp, Warning, TEXT("Asize %i"), test_board_state.all_cards.Num());
}

void AFartifactGameMode::PostLogin(APlayerController* player)
{
	Super::PostLogin(player);

	((AFartifactPlayerController*)player)->player_id = player->PlayerState->PlayerId;
}