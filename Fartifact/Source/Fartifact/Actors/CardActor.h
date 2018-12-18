// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/BoardPositionActor.h"
#include "CardManager.h"


#include "CardActor.generated.h"

UCLASS()
class FARTIFACT_API ACardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	FCard* CardInformation = nullptr;
public:
	EPosition Position;

protected:

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* PreviewWidget = nullptr;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneComponent = nullptr;

};
