// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CardManager.h"

struct BadMove
{
	FString reason;
};

struct FMoveResult
{
	FBoardState result;
	bool success = false;
	FString reason;

	FMoveResult(FBoardState res)
	{
		result = res;
		success = true;
	}

	FMoveResult(const BadMove& t)
	{
		success = false;
		reason = t.reason;
	}
};

/**
 * 
 */
class FARTIFACT_API MoveHandler
{
public:
	MoveHandler();
	~MoveHandler();

	static FMoveResult Play(FBoardState in, const FCardMove& to_play, uint64 player_id);
};
