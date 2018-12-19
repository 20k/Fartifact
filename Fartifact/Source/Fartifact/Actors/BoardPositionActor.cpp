// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardPositionActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CardWidget.h"
#include "Engine/World.h"
#include "FartifactGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"

ABoardPositionActor::ABoardPositionActor()
{
	ConstructorHelpers::FClassFinder<UUserWidget> CardClass(TEXT("/Game/WidgetBlueprints/CardWidgetBP"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial(TEXT("/Game/Materials/CardWidgetMaterial"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PreviewCardComponent"));
	SetRootComponent(SceneComponent);
	PreviewWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));

	


	PreviewWidget->SetupAttachment(RootComponent);
	PreviewWidget->SetDrawAtDesiredSize(true);
	PreviewWidget->SetWidgetClass(CardClass.Class);
	PreviewWidget->SetMaterial(0, FoundMaterial.Object);
	PreviewWidget->SetRelativeRotation(FRotator(0, 180, 0));
	PreviewWidget->SetRelativeScale3D(FVector(1, 0.2, 0.2));
	PreviewWidget->SetHiddenInGame(true);

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(RootComponent);
	HitBox->SetRelativeScale3D(FVector(0.1, 2.3, 3.2));


}

void ABoardPositionActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		MyGameInstance = Cast<UFartifactGameInstance>(GetWorld()->GetGameInstance());
	}
	if (MyGameInstance)
	{
		switch (Position)
		{
			case EPosition::HandYours:
				MyGameInstance->HandYoursPositions[MyIndex] = this;
				break;
			case EPosition::HandTheirs:
				MyGameInstance->HandTheirsPositions[MyIndex] = this;
				break;
			case EPosition::BoardYours:
				MyGameInstance->BoardYoursPositions[MyIndex] = this;
				break;
			case EPosition::BoardTheirs:
				MyGameInstance->BoardTheirsPositions[MyIndex] = this;
				break;
		}
	}

}

