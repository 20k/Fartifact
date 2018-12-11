// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConsoleWidget.generated.h"

/**
 * 
 */
UCLASS()
class FARTIFACT_API UConsoleWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	void UpdateConsoleText(FString ACommand);

protected:
	virtual bool Initialize();


	UFUNCTION()
	void PreSendCommand();

private:


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ConsoleText;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ConsoleTextBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* SendButton;
	
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ConsoleScrollBox;
	
	TArray<FString> CommandHistory;

};
