// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "BoardPositionActor.generated.h"

UENUM()		//"BlueprintType" is essential to include
enum class EPosition : uint8
{
	HandYours	UMETA(DisplayName = "HandYours"),
	HandTheirs	UMETA(DisplayName = "HandTheirs"),
	BoardYours	UMETA(DisplayName = "BoardYours"),
	BoardTheirs	UMETA(DisplayName = "BoardTheirs")

};


/**
 * 
 */


UCLASS()
class FARTIFACT_API ABoardPositionActor : public AActor
{
	GENERATED_BODY()

public:
	ABoardPositionActor();

	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere)
	EPosition Position = EPosition::HandYours;

	UPROPERTY(EditAnywhere)
	int MyIndex = 0;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* PreviewWidget = nullptr;

	UPROPERTY(EditAnywhere)
		class UTimelineComponent* MyTimeline;

	UPROPERTY(EditAnywhere)
		class UCurveFloat* FloatCurve;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;


	UPROPERTY(EditAnywhere)
	class UBoxComponent* HitBox = nullptr;

	UPROPERTY(EditAnywhere)
	class USceneComponent* SceneComponent = nullptr;

protected:
	class UFartifactGameInstance* MyGameInstance = nullptr;




};
