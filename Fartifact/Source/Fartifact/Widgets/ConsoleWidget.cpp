// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleWidget.h"
#include "Engine/World.h"
#include "FartifactPlayerController.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "FartifactGameState.h"




bool UConsoleWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(SendButton != nullptr)) return false;
	SendButton->OnClicked.AddDynamic(this, &UConsoleWidget::PreSendCommand);


	return true;
}

void UConsoleWidget::UpdateConsoleText(FString ACommand)
{
	CommandHistory.Add(ACommand);

	FString ConsoleString;
	for (FString Com : CommandHistory)
	{
		ConsoleString.Append(Com);
		ConsoleString.Append("\n");
	}

	ConsoleText->SetText(FText::FromString(ConsoleString));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	//ConsoleText->SetText(FText::FromString(Cast<AFartifactGameState>(World->GetGameState())->CommandList));
	ConsoleScrollBox->ScrollToEnd();
}

void UConsoleWidget::PreSendCommand()
{

	FString CommandString = ConsoleTextBox->Text.ToString();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	AFartifactPlayerController* MyController = Cast<AFartifactPlayerController>(World->GetFirstPlayerController());

	MyController->PreSendCommand(CommandString);
}