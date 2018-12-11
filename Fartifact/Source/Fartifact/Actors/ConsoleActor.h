// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConsoleActor.generated.h"

UCLASS()
class FARTIFACT_API AConsoleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConsoleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendCommand(const FString& ACommand);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void NetMulticast_SendCommand(const FString& ACommand);

};
