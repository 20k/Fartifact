// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CardManager.h"
#include "FartifactPlayerController.generated.h"

USTRUCT()
struct FCardArray
{
	GENERATED_BODY()

		UPROPERTY()
		TArray<class ACardActor*> Pile;
};

UCLASS()
class AFartifactPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFartifactPlayerController();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void CommandToServer(const FString& ACommand);


	void PreSendCommand(FString ACommand);

	UFUNCTION(Server, Reliable, WithValidation)
	void FetchGameStateFromServer();

	UFUNCTION(Client, Reliable)
	void ReceiveGameState(FBoardState board_state);

	void MakeBoardChanges(FBoardState BoardState);


protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;

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
	int64 player_id;
	int HandSize = 8;
	int BoardSize = 8;

public:
	class UConsoleWidget* ConsoleWidget = nullptr;

	FBoardState CurrentBoardState;
	//array of actors for each hand
	TArray<class ACardActor*> CurrentHandYoursCards;
	TArray<class ACardActor*> CurrentHandTheirsCards;

	//array of arrays for each pile on the board
	TArray<FCardArray*> CurrentBoardYoursCards;
	TArray<FCardArray*> CurrentBoardTheirsCards;

};


