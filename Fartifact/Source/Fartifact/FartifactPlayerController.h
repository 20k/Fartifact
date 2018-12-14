// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CardManager.h"
#include "FartifactPlayerController.generated.h"

UCLASS()
class AFartifactPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFartifactPlayerController();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void CommandToServer(const FString& ACommand);

	UFUNCTION(Server, Reliable, WithValidation)
	void DrawCard();

	UFUNCTION(Server, Reliable, WithValidation)
	void PlayCard(int pcard_offset);


	void PreSendCommand(FString ACommand);

	UFUNCTION(Server, Reliable, WithValidation)
	void FetchGameStateFromServer();

	UFUNCTION(Client, Reliable)
	void ReceiveGameState(FBoardState board_state);

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();



	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	void ToggleConsole();

protected:
	TSubclassOf<class UUserWidget> ConsoleClass = nullptr;
	class AFartifactGameStateBase* MyGameState = nullptr;
	class UWorld* MyWorld = nullptr;

public:
	class UConsoleWidget* ConsoleWidget = nullptr;

};


