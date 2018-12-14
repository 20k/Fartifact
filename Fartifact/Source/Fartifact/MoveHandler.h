// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CardManager.h"

struct BadMovet {};

struct FMoveResult
{
	FBoardState result;
	bool success = false;

	FMoveResult(FBoardState res)
	{
		result = res;
		success = true;
	}

	FMoveResult(const BadMovet&)
	{
		success = false;
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
