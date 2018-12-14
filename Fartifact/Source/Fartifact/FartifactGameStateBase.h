// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FartifactGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FARTIFACT_API AFartifactGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation) // for player to player rpc you need to first call the message on the server
	virtual void CommandToServer(const FString& ACommand); // first rpc for the server

	UFUNCTION(NetMulticast, Reliable, WithValidation) // then the server calls the function with a multicast that executes on all clients and the server
	virtual void CommandToMulti(const FString& ACommand); // second rpc for all the clients

private:
	UWorld* MyWorld = nullptr;


};
