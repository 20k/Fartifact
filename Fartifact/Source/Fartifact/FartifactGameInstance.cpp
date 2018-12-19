// Fill out your copyright notice in the Description page of Project Settings.

#include "FartifactGameInstance.h"

#include "Actors/BoardPositionActor.h"


UFartifactGameInstance::UFartifactGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HandYoursPositions.SetNum(8);
	HandTheirsPositions.SetNum(8);
	BoardYoursPositions.SetNum(8);
	BoardTheirsPositions.SetNum(8);
}
