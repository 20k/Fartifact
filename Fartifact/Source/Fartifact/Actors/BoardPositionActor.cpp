// Fill out your copyright notice in the Description page of Project Settings.

#include "BoardPositionActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CardWidget.h"
#include "Components/WidgetComponent.h"
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

	


	PreviewWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	PreviewWidget->SetDrawAtDesiredSize(true);
	PreviewWidget->SetWidgetClass(CardClass.Class);
	PreviewWidget->SetMaterial(0, FoundMaterial.Object);
	PreviewWidget->SetRelativeRotation(FRotator(0, 180, 0));
	PreviewWidget->SetRelativeScale3D(FVector(1, 0.2, 0.2));

	/*HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	HitBox->SetRelativeScale3D(FVector(0.1, 2.3, 3.2));*/


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
				MyGameInstance->HandYoursPositions.Insert(this, MyIndex);
			case EPosition::HandTheirs:
				MyGameInstance->HandYoursPositions.Insert(this, MyIndex);
			case EPosition::BoardYours:
				MyGameInstance->BoardYoursPositions.Insert(this, MyIndex);
			case EPosition::BoardTheirs:
				MyGameInstance->BoardTheirsPositions.Insert(this, MyIndex);
		}
	}

}

