// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FartifactGameMode.h"
#include "FartifactPlayerController.h"
#include "FartifactCharacter.h"
#include "CameraPawn.h"

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
	DefaultPawnClass = ACameraPawn::StaticClass();

	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

void AFartifactGameMode::BeginPlay()
{
	
}

void AFartifactGameMode::StartGame()
{
	FCardManager test_deck;

	FCard tcard1;
	FCard tcard2;
	FCard tcard3;

	tcard1.which = (int)FCard::type::AMERICAN_DOLLAR;
	tcard2.which = (int)FCard::type::PESO;
	tcard3.which = (int)FCard::type::EURO;

	test_deck.Add({ tcard1, tcard2, tcard3 });

	//board_state.AddPlayerAndDeck(0, test_deck);
	//board_state.AddPlayerAndDeck(1, test_deck);

	for (auto id : player_ids)
	{
		board_state.AddPlayerAndDeck(id, test_deck);
	}

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *board_state.Debug());
	//UE_LOG(LogTemp, Warning, TEXT("Asize %i"), board_state.all_cards.Num());
}

void AFartifactGameMode::PostLogin(APlayerController* player)
{
	Super::PostLogin(player);

	uint64 player_id = player->PlayerState->PlayerId;

	((AFartifactPlayerController*)player)->player_id = player_id;

	player_ids.Add(player_id);

	UE_LOG(LogTemp, Warning, TEXT("Gained player with id %i"), (int32)player_id);

	if (player_ids.Num() == 2)
	{
		StartGame();
	}
}