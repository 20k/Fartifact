// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FartifactPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "FartifactCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Actors/CardActor.h"
#include "FartifactGameInstance.h"
#include "CardManager.h"

#include "FartifactGameStateBase.h"
#include "Widgets/ConsoleWidget.h"
#include "Engine/World.h"
#include "FartifactGameMode.h"

AFartifactPlayerController::AFartifactPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	ConstructorHelpers::FClassFinder<UUserWidget> ConsoleWidgetBPClass(TEXT("/Game/WidgetBlueprints/ConsoleWidget"));
	ConsoleClass = ConsoleWidgetBPClass.Class;

	CurrentHandYoursCards.SetNum(8);
	CurrentHandTheirsCards.SetNum(8);
	CurrentBoardYoursCards.SetNum(8);
	CurrentBoardTheirsCards.SetNum(8);

	
}

void AFartifactPlayerController::BeginPlay()
{

	MyWorld = GetWorld();
	if (!MyWorld)
		return;

	MyGameState = Cast<AFartifactGameStateBase>(MyWorld->GetGameState());
	MyGameInstance = Cast<UFartifactGameInstance>(MyWorld->GetGameInstance());



	if (IsLocalPlayerController())
	{
		if (ConsoleClass != nullptr)
		{
			ConsoleWidget = Cast<UConsoleWidget>(CreateWidget<UUserWidget>(this, ConsoleClass));
			if (ConsoleWidget)
				ConsoleWidget->AddToViewport();
		}
	}
}

//Here we're going to lay out the board based on a board state
void AFartifactPlayerController::MakeBoardChanges(FBoardState BoardState)
{
	if (!MyWorld)
		return;
	
	TArray<FOwnedCardManager> PlayerHands = BoardState.player_hands;
	TArray<FOwnedCardManager> BoardStates = BoardState.board_states;
	

	for (int i = PlayerHands[0].cards.cards.Num(); i < CurrentHandYoursCards.Num(); i++)
	{
		if (CurrentHandYoursCards[i] != nullptr)
		{
			CurrentHandYoursCards[i]->Destroy();
			CurrentHandYoursCards[i] = nullptr;
		}
	}

	for (int i = PlayerHands[1].cards.cards.Num(); i < CurrentHandTheirsCards.Num(); i++)
	{
		if (CurrentHandTheirsCards[i] != nullptr)
		{
			CurrentHandTheirsCards[i]->Destroy();
			CurrentHandTheirsCards[i] = nullptr;
		}
	}



	//First we iterate through the two sets of hands and spawn actors based on the cards
	for (int i = 0; i < PlayerHands.Num(); i++)
	{
		FOwnedCardManager AHand = PlayerHands[i];

		//HANDS
		//then we go through the cards in the updated state and recreate or update any differences
		for (int j = 0; j < AHand.cards.cards.Num(); j++)
		{
			if (j <= MyGameInstance->HandYoursPositions.Num())
				return;

			FCard ACard = AHand.cards.cards[j];

			//your hand
			if (i == 0)
			{
				if (CurrentHandYoursCards[j] != nullptr)
				{
					if (!(ACard == *(CurrentHandYoursCards[j]->CardInformation)))
					{
						CurrentHandYoursCards[j]->Destroy();
						CurrentHandYoursCards[j] = MyWorld->SpawnActor<ACardActor>(MyGameInstance->HandYoursPositions[j]->GetActorLocation(), MyGameInstance->HandYoursPositions[j]->GetActorRotation());

					}
				}
				else
				{
					CurrentHandYoursCards[j] = MyWorld->SpawnActor<ACardActor>(MyGameInstance->HandYoursPositions[j]->GetActorLocation(), MyGameInstance->HandYoursPositions[j]->GetActorRotation());
				}
					
			}
			//their hand
			else if (i == 1)
			{
				if (CurrentHandTheirsCards[j] != nullptr)
				{
					if (!(ACard == *(CurrentHandTheirsCards[j]->CardInformation)))
					{
						CurrentHandTheirsCards[j]->Destroy();
						CurrentHandTheirsCards[j] = MyWorld->SpawnActor<ACardActor>(MyGameInstance->HandTheirsPositions[j]->GetActorLocation(), MyGameInstance->HandTheirsPositions[j]->GetActorRotation());

					}
				}
				else
				{
					CurrentHandTheirsCards[j] = MyWorld->SpawnActor<ACardActor>(MyGameInstance->HandTheirsPositions[j]->GetActorLocation(), MyGameInstance->HandTheirsPositions[j]->GetActorRotation());
				}
			}
		}
	}



	//BOARD
	// Now we go through each card in the board and place them based on their current ownership
	for (int i = 0; i < BoardStates.Num(); i++)
	{
		FOwnedCardManager ABoardSlot = BoardStates[i];

		//SLOTS
		//now we go through managers for each card/empty position on the board and update the cards
		if (ABoardSlot.owner == 0) // CHANGE THIS TO CHECK WHO OWNS INSTEAD OF == 0
		{
			for (int j = 0; j < ABoardSlot.cards.cards.Num(); j++)
			{
				if (j <= MyGameInstance->HandYoursPositions.Num())
					return;

				FCard ACard = ABoardSlot.cards.cards[j];
				if (CurrentBoardYoursCards[i]->Pile[j] != nullptr)
				{
					if (!(ACard == *(CurrentBoardYoursCards[i]->Pile[j]->CardInformation)))
					{
						CurrentHandYoursCards[j] = MyWorld->SpawnActor<ACardActor>(MyGameInstance->HandYoursPositions[j]->GetActorLocation(), MyGameInstance->HandYoursPositions[j]->GetActorRotation());
						CurrentHandYoursCards[j]->Destroy();
					}
				}
				else
				{
					CurrentHandYoursCards[j] = MyWorld->SpawnActor<ACardActor>(MyGameInstance->HandYoursPositions[j]->GetActorLocation(), MyGameInstance->HandYoursPositions[j]->GetActorRotation());
					
				}
			}

		}
	}
}

void AFartifactPlayerController::PreSendCommand(FString ACommand)
{
	
	if (!MyWorld || !MyGameState)
		return;


	if (GetPawn()->Role == ROLE_Authority)
	{
		FString ExtraString = "Server: ";
		ExtraString.Append(ACommand);


		MyGameState->CommandToMulti(ExtraString);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ACommand);

		if (ACommand == "Fetch")
		{
			FetchGameStateFromServer();
		}

		FString ExtraString = "Client: ";
		ExtraString.Append(ACommand);

		CommandToServer(ExtraString);
	}
}

bool AFartifactPlayerController::FetchGameStateFromServer_Validate()
{
	return true;
}

void AFartifactPlayerController::FetchGameStateFromServer_Implementation()
{
	if (GetWorld() == nullptr)
		return;

	if (GetWorld()->GetAuthGameMode() == nullptr)
		return;

	auto game_mode = GetWorld()->GetAuthGameMode();

	FBoardState all = ((AFartifactGameMode*)game_mode)->test_board_state;

	ReceiveGameState(all);
}

void AFartifactPlayerController::ReceiveGameState_Implementation(FBoardState board_state)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(board_state.Debug()));
}

void AFartifactPlayerController::CommandToServer_Implementation(const FString& ACommand)
{
	if (MyGameState)
	{
		MyGameState->CommandToMulti(ACommand);
	}

}

bool AFartifactPlayerController::CommandToServer_Validate(const FString& ACommand)
{
	return true;
}

void AFartifactPlayerController::PlayerTick(float DeltaTime)
{
}

void AFartifactPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
	InputComponent->BindAction("ToggleConsole", IE_Released, this, &AFartifactPlayerController::ToggleConsole);
}

void AFartifactPlayerController::ToggleConsole()
{
	if (ConsoleWidget != nullptr)
	{
		
		if (!ConsoleWidget->IsInViewport())
		{
			ConsoleWidget->AddToViewport();

		}
		else
		{
			ConsoleWidget->RemoveFromViewport();
		}
	}
}
