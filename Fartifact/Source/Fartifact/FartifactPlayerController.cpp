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
#include "MoveHandler.h"

AFartifactPlayerController::AFartifactPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	ConstructorHelpers::FClassFinder<UUserWidget> ConsoleWidgetBPClass(TEXT("/Game/WidgetBlueprints/ConsoleWidget"));
	ConsoleClass = ConsoleWidgetBPClass.Class;



	/*YourHandActors.SetNum(8);
	TheirHandActors.SetNum(8);
	YourBoardActors.SetNum(8);
	TheirBoardActors.SetNum(8);*/

	
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
	FetchGameStateFromServer();
}

void AFartifactPlayerController::MakeHandChanges(TArray<FCard> AHand, uint64 APlayer)
{
	TArray<ACardActor*>* HandActors = nullptr;
	TArray<ABoardPositionActor*>* PositionActors = nullptr;

	if (APlayer == player_id)
	{
		HandActors = &YourHandActors;
		PositionActors = &MyGameInstance->HandYoursPositions;
	}
	else
	{
		HandActors = &TheirHandActors;
		PositionActors = &MyGameInstance->HandTheirsPositions;
	}

	if (!HandActors || !PositionActors)
		return;


	//first we delete any extra actors currently on the board
	if (HandActors->Num() > AHand.Num())
	{
		int TotalActors = HandActors->Num();
		for (int i = AHand.Num(); i < TotalActors; i++)
		{
			(*HandActors)[i]->Destroy();
		}
		HandActors->SetNum(AHand.Num(), false);
	}

	//now we go through the hand state and remake any actors that arent correct
	for (int i = 0; i < AHand.Num(); i++)
	{
		if (i < HandActors->Num())
		{
			if (!(AHand[i] == (*HandActors)[i]->CardInformation))
			{
				(*HandActors)[i]->Destroy();
				(*HandActors)[i] = MyWorld->SpawnActor<ACardActor>((*PositionActors)[i]->GetActorLocation(), (*PositionActors)[i]->GetActorRotation());
				(*HandActors)[i]->CardInformation = AHand[i];
			}
		}
		else
		{
			ACardActor* ACardTemp = MyWorld->SpawnActor<ACardActor>((*PositionActors)[i]->GetActorLocation(), (*PositionActors)[i]->GetActorRotation());
			ACardTemp->CardInformation = AHand[i];
			HandActors->Add(ACardTemp);
		}
	}

}

void AFartifactPlayerController::MakeBoardChanges(TArray<FOwnedCardManager> ABoardSide, uint64 APlayer)
{
	TArray<FCardArray> BoardActors;
	TArray<ABoardPositionActor*>* PositionActors = nullptr;

	if (APlayer == player_id)
	{
		BoardActors = YourBoardActors;
		PositionActors = &MyGameInstance->BoardYoursPositions;
	}
	else
	{
		BoardActors = TheirBoardActors;
		PositionActors = &MyGameInstance->BoardTheirsPositions;
	}

	if (!PositionActors)
		return;

	//first we delete any extra piles and then remove extra actors in the remaining piles
	if (BoardActors.Num() > ABoardSide.Num())
	{
		int TotalPiles = BoardActors.Num();

		for (int i = ABoardSide.Num(); i < TotalPiles; i++)
		{
			for (ACardActor* ACard : BoardActors[i].Pile)
			{
				ACard->Destroy();

			}
			BoardActors[i].Pile.Empty();
		}

	}
	BoardActors.SetNum(ABoardSide.Num(), false);


	//now we delete any extra actors in each pile
	for (int i = 0; i < BoardActors.Num(); i++)
	{
		if (BoardActors[i].Pile.Num() > ABoardSide[i].cards.cards.Num())
		{
			int TotalActors = BoardActors[i].Pile.Num();
			for (int j = ABoardSide[i].cards.cards.Num(); j < TotalActors; j++)
			{
				BoardActors[i].Pile[j]->Destroy();
			}
			BoardActors[i].Pile.SetNum(ABoardSide[i].cards.cards.Num(), false);
		}
	}


	//now we go through the differences between the board state and the current actors and remake or add any necessary actors

	for (int i = 0; i < ABoardSide.Num(); i++)
	{
		for (int j = 0; j < ABoardSide[i].cards.cards.Num(); j++)
		{
			if (j < BoardActors[i].Pile.Num())
			{
				if (!(BoardActors[i].Pile[j]->CardInformation == ABoardSide[i].cards.cards[j]))
				{
					BoardActors[i].Pile[j]->Destroy();
					BoardActors[i].Pile[j] = MyWorld->SpawnActor<ACardActor>((*PositionActors)[i]->GetActorLocation(), (*PositionActors)[i]->GetActorRotation());
					BoardActors[i].Pile[j]->CardInformation = ABoardSide[i].cards.cards[j];
				}
			}
			else
			{
				ACardActor* TempActor = MyWorld->SpawnActor<ACardActor>((*PositionActors)[i]->GetActorLocation(), (*PositionActors)[i]->GetActorRotation());
				TempActor->CardInformation = ABoardSide[i].cards.cards[j];
				BoardActors[i].Pile.Add(TempActor);
			}
		}
	}
}


//Here we're going to lay out the board based on a board state
void AFartifactPlayerController::MakeStateChanges(FBoardState BoardState)
{
	if (!MyWorld)
		return;
	
	TArray<FCard> YourHand = (BoardState.GetCardsFor(FBoardState::board_states::HANDS, player_id))[0].cards.cards;
	TArray<FCard> TheirHand = (BoardState.GetCardsFor(FBoardState::board_states::HANDS, their_player_id))[0].cards.cards;
	TArray<FOwnedCardManager> YourBoard = BoardState.GetCardsFor(FBoardState::board_states::BOARD, player_id);
	TArray<FOwnedCardManager> TheirBoard = BoardState.GetCardsFor(FBoardState::board_states::BOARD, their_player_id);

	MakeHandChanges(YourHand, player_id);
	MakeHandChanges(TheirHand, their_player_id);
	MakeBoardChanges(YourBoard, player_id);
	MakeBoardChanges(TheirBoard, their_player_id);
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
			PreFetchGameStateFromServer();
		}

		if (ACommand == "Draw")
		{
			DrawCard();
		}

		if (ACommand == "Play")
		{
			PlayCard(0);
		}

		FString ExtraString = FString::Printf(TEXT("Client %d : %s"), player_id, *ACommand);

		
		PreFetchGameStateFromServer();
		CommandToServer(ExtraString);
	}
}

void AFartifactPlayerController::PreFetchGameStateFromServer_Implementation()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AFartifactPlayerController* AController = nullptr;
		AController = Cast<AFartifactPlayerController>(Iterator->Get());
		if (AController)
		{
			AController->FetchGameStateFromServer();

		}
	}
}

bool AFartifactPlayerController::PreFetchGameStateFromServer_Validate()
{
	return true;
}

bool AFartifactPlayerController::FetchGameStateFromServer_Validate()
{
	return true;
}

void AFartifactPlayerController::FetchGameStateFromServer_Implementation()
{
	if (GetWorld() == nullptr)
		return;
	
	auto game_mode = GetWorld()->GetAuthGameMode();

	if (game_mode == nullptr)
		return;

	FBoardState all = ((AFartifactGameMode*)game_mode)->board_state;

	//UE_LOG(LogTemp, Warning, TEXT("%i"), all.all_cards.Num());

	//UE_LOG(LogTemp, Warning, TEXT("Server: %i"), all.all_cards[(int)FBoardState::board_states::DECKS].owned.Num());

	ReceiveGameState(all.HideByVisibility(player_id), player_id);
}

void AFartifactPlayerController::ReceiveGameState_Implementation(FBoardState board_state, uint64 my_id)
{
	player_id = my_id;

	for (uint64 Player : board_state.players)
	{
		if (Player != player_id)
		{
			their_player_id = Player;
			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%i"), board_state.all_cards.Num());
	UE_LOG(LogTemp, Warning, TEXT("%i"), board_state.all_cards[(int)FBoardState::board_states::DECKS].owned.Num());

	UE_LOG(LogTemp, Warning, TEXT("%s"), *(board_state.Debug()));

	MakeStateChanges(board_state);
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

bool AFartifactPlayerController::DrawCard_Validate()
{
	return true;
}

void AFartifactPlayerController::DrawCard_Implementation()
{
	if (GetWorld() == nullptr)
		return;

	auto game_mode = GetWorld()->GetAuthGameMode();

	if (game_mode == nullptr)
		return;

	FBoardState& all = ((AFartifactGameMode*)game_mode)->board_state;

	UE_LOG(LogTemp, Warning, TEXT("PLAYER WITH ID %i"), (int32)player_id);

	FCardMove mv;
	///0 is a stand in for whatever my id  is
	mv.MakeDraw(all, player_id);

	FMoveResult res = MoveHandler::Play(all, mv, player_id);

	UE_LOG(LogTemp, Warning, TEXT("Server success %i %s\n"), res.success, *res.reason);

	if (!res.success)
		return;
	else
		all = res.result;
}

bool AFartifactPlayerController::PlayCard_Validate(int pcard_offset)
{
	return true;
}

void AFartifactPlayerController::PlayCard_Implementation(int pcard_offset)
{
	if (GetWorld() == nullptr)
		return;

	auto game_mode = GetWorld()->GetAuthGameMode();

	if (game_mode == nullptr)
		return;

	FBoardState& all = ((AFartifactGameMode*)game_mode)->board_state;

	FCardMove mv;
	///0 is a stand in for whatever my id  is
	mv.MakePlay(all, player_id, pcard_offset);

	FMoveResult res = MoveHandler::Play(all, mv, player_id);

	if (!res.success)
		return;
	else
		all = res.result;
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
