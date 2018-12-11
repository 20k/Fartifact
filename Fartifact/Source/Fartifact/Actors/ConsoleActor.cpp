// Fill out your copyright notice in the Description page of Project Settings.

#include "ConsoleActor.h"
#include "FartifactPlayerController.h"

#include "Widgets/ConsoleWidget.h"
#include "Engine/World.h"


// Sets default values
AConsoleActor::AConsoleActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
}

void AConsoleActor::Server_SendCommand_Implementation(const FString& ACommand)
{
	NetMulticast_SendCommand(ACommand);

}

bool AConsoleActor::Server_SendCommand_Validate(const FString& ACommand)
{
	return true;
}

void AConsoleActor::NetMulticast_SendCommand_Implementation(const FString& ACommand)
{
	Cast<AFartifactPlayerController>(GetWorld()->GetFirstPlayerController())->ConsoleWidget->UpdateConsoleText(ACommand);
}

bool AConsoleActor::NetMulticast_SendCommand_Validate(const FString& ACommand)
{
	return true;

}


// Called when the game starts or when spawned
void AConsoleActor::BeginPlay()
{
	Super::BeginPlay();

}



