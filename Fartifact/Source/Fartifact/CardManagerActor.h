// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <string>
#include "CardManagerActor.generated.h"

UCLASS()
class FARTIFACT_API ACardActor : public AActor
{
	GENERATED_BODY()

public:

	ACardActor();

	static std::vector<std::string> short_names;

	enum class type
	{
		UNKNOWN = 0,
		AMERICAN_DOLLAR,
		POUND,
		DONG,
		RUBLE,
		EURO,
		PESO,
		RUPEE,
		BUTTONS,
		CANADIAN_DOLLAR,
		AUSTRALIAN_DOLLARYDOO,
		ZIMBABWEAN_DOLLARS,
		YUAN,
		COUNT,
		NONE
	};

	enum class visibility
	{
		OWNER, ///eg hand, face down cards on the board owned by someone
		ALL, ///visible to everyone, face up card
		NONE, ///not visible to anyone, face down neutral
	};

	std::string GetShortName();

	type which = type::NONE;
	visibility visible = visibility::NONE;
	uint64_t owner_id = 0; ///placeholder

	bool IsOwnedBy(uint64_t puser_id);
	bool IsVisibleTo(uint64_t puser_id);
};

UCLASS()
class FARTIFACT_API ACardManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardManagerActor();
	~ACardManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	std::vector<ACardActor*> cards;

	void Add(ACardActor* c);
	void Add(std::vector<ACardActor*>& pcards);
	ACardActor* Remove(uint32_t index);
	ACardActor* Fetch(uint32_t index);
	void Clear();

	///returns exactly the same deck of cards, except hides the ones that owner_id is not meant to be able to see
	//CardManager HideByVisibility(uint64_t powner_id);

	std::string Debug();

	template<typename T>
	void Shuffle(T& engine)
	{
		std::shuffle(std::begin(cards), std::end(cards), engine);
	}

	//static CardManager Merge(const CardManager& c1, const CardManager& c2);
};
