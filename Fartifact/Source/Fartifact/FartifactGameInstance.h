// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FartifactGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FARTIFACT_API UFartifactGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	TArray<class ABoardPositionActor*> HandYoursPositions;
	TArray<class ABoardPositionActor*> HandTheirsPositions;
	TArray<class ABoardPositionActor*> BoardYoursPositions;
	TArray<class ABoardPositionActor*> BoardTheirsPositions;
	
};
