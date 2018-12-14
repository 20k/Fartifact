// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "FartifactPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "FartifactCharacter.h"
#include "UObject/ConstructorHelpers.h"

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
}

void AFartifactPlayerController::BeginPlay()
{

	MyWorld = GetWorld();
	if (!MyWorld)
		return;

	MyGameState = Cast<AFartifactGameStateBase>(MyWorld->GetGameState());


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

	if (game_mode == nullptr)
		return;

	FBoardState all = ((AFartifactGameMode*)game_mode)->test_board_state;

	UE_LOG(LogTemp, Warning, TEXT("%i"), all.all_cards.Num());

	UE_LOG(LogTemp, Warning, TEXT("Server: %i"), all.all_cards[(int)FBoardState::board_states::DECKS].owned.Num());

	ReceiveGameState(all.HideByVisibility(0));
}

void AFartifactPlayerController::ReceiveGameState_Implementation(FBoardState board_state)
{
	UE_LOG(LogTemp, Warning, TEXT("%i"), board_state.all_cards.Num());
	UE_LOG(LogTemp, Warning, TEXT("%i"), board_state.all_cards[(int)FBoardState::board_states::DECKS].owned.Num());

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
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AFartifactPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AFartifactPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AFartifactPlayerController::OnSetDestinationReleased);
	InputComponent->BindAction("ToggleConsole", IE_Released, this, &AFartifactPlayerController::ToggleConsole);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFartifactPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFartifactPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AFartifactPlayerController::OnResetVR);
}

void AFartifactPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFartifactPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AFartifactCharacter* MyPawn = Cast<AFartifactCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AFartifactPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AFartifactPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AFartifactPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AFartifactPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AFartifactPlayerController::ToggleConsole()
{
	if (ConsoleWidget != nullptr)
	{
		//ConsoleWidget->AddToViewport();


		//if (!ConsoleWidget->IsInViewport())
		//{
		//}
		//else
		//{
		//	//ConsoleWidget->RemoveFromViewport();
		//}
	}
}
