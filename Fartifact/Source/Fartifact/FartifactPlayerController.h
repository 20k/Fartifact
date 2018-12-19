// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CardManager.h"
#include "Actors/CardActor.h"

#include "FartifactPlayerController.generated.h"

USTRUCT()
struct FCardArray
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<ACardActor*> Pile;

	//FCardArray();
};

UCLASS()
class AFartifactPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFartifactPlayerController();

	uint64 player_id = 0;
	uint64 their_player_id = 0;
	UFUNCTION(Server, Reliable, WithValidation)
	void CommandToServer(const FString& ACommand);

	UFUNCTION(Server, Reliable, WithValidation)
	void DrawCard();

	UFUNCTION(Server, Reliable, WithValidation)
	void PlayCard(int pcard_offset);

	void PreSendCommand(FString ACommand);

	UFUNCTION(Server, Reliable, WithValidation)
	void FetchGameStateFromServer();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void PreFetchGameStateFromServer();

	UFUNCTION(Client, Reliable)
	void ReceiveGameState(FBoardState board_state, uint64 my_id);

	void MakeStateChanges(FBoardState BoardState);


protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;

	void MakeHandChanges(TArray<FCard> AHand, uint64 APlayer);
	void MakeBoardChanges(TArray<FOwnedCardManager> ABoardSide, uint64 APlayer);
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface


	void ToggleConsole();

protected:
	TSubclassOf<class UUserWidget> ConsoleClass = nullptr;
	class AFartifactGameStateBase* MyGameState = nullptr;
	class UWorld* MyWorld = nullptr;
	class UFartifactGameInstance* MyGameInstance = nullptr;
	int HandSize = 8;
	int BoardSize = 8;

public:
	class UConsoleWidget* ConsoleWidget = nullptr;

	FBoardState CurrentBoardState;
	//array of actors for each hand
	TArray<class ACardActor*> YourHandActors;
	TArray<class ACardActor*> TheirHandActors;

	//array of arrays for each pile on the board
	UPROPERTY()
	TArray<FCardArray> YourBoardActors;
	UPROPERTY()
	TArray<FCardArray> TheirBoardActors;

};


