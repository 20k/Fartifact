// Fill out your copyright notice in the Description page of Project Settings.

#include "CardActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CardWidget.h"
#include "Materials/Material.h"
#include "BoardPositionActor.h"
#include "CardManager.h"

// Sets default values
ACardActor::ACardActor()
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

	

}

// Called when the game starts or when spawned
void ACardActor::BeginPlay()
{
	Super::BeginPlay();
	
}


