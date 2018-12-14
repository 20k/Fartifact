// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CardManager.h"
#include "FartifactGameMode.generated.h"

UCLASS(minimalapi)
class AFartifactGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFartifactGameMode();

	void BeginPlay() override;

	void PostLogin(APlayerController* player);
	void StartGame();

	TArray<uint64> player_ids;
	FBoardState board_state;
};