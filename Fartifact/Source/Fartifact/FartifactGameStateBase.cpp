// Fill out your copyright notice in the Description page of Project Settings.

#include "FartifactGameStateBase.h"
#include "Actors/ConsoleActor.h"
#include "Widgets/ConsoleWidget.h"
#include "FartifactPlayerController.h"



#include "Engine/World.h"
void AFartifactGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	MyWorld = GetWorld();
}

void AFartifactGameStateBase::CommandToMulti_Implementation(const FString& ACommand)
{
	if (MyWorld != nullptr)
	{
		UConsoleWidget* ConsoleWidget = Cast<AFartifactPlayerController>(MyWorld->GetFirstPlayerController())->ConsoleWidget;
		if (ConsoleWidget)
		{
			ConsoleWidget->UpdateConsoleText(ACommand);
		}
	}
}

bool AFartifactGameStateBase::CommandToMulti_Validate(const FString& ACommand)
{
	return true;
}

void AFartifactGameStateBase::CommandToServer_Implementation(const FString& ACommand)
{
	CommandToMulti(ACommand);
}

bool AFartifactGameStateBase::CommandToServer_Validate(const FString& ACommand)
{
	return true;
}
