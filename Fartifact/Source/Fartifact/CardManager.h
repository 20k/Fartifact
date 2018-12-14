// copyrite notuc ur a bum

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include <vector>
#include <string>
#include <stdint.h>

#include "Runtime/Core/Public/Containers/Map.h"

#include "CardManager.generated.h"

USTRUCT()
struct FCard
{
	GENERATED_BODY()

	static std::vector<FString> short_names;

	UENUM()
	enum class type : uint8
	{
		UNKNOWN = 0 UMETA(),
		AMERICAN_DOLLAR UMETA(),
		POUND UMETA(),
		DONG UMETA(),
		RUBLE UMETA(),
		EURO UMETA(),
		PESO UMETA(),
		RUPEE UMETA(),
		BUTTONS UMETA(),
		CANADIAN_DOLLAR UMETA(),
		AUSTRALIAN_DOLLARYDOO UMETA(),
		ZIMBABWEAN_DOLLARS UMETA(),
		YUAN UMETA(),
		COUNT UMETA(),
		NONE UMETA()
	};

	UENUM()
	enum class visibility : uint8
	{
		OWNER, ///eg hand, face down cards on the board owned by someone
		ALL, ///visible to everyone, face up card
		NONE, ///not visible to anyone, face down neutral
	};

	FString GetShortName();
	FString GetLongName();

	UPROPERTY(Transient)
	int which = (int)type::NONE;
	UPROPERTY(Transient)
	int visible = (int)visibility::NONE;
	
	uint64_t owner_id = 0;

	bool IsOwnedBy(uint64_t puser_id);
	bool IsVisibleTo(uint64_t puser_id);
};

/**
 * Collection of cards, a logical pile of some description
 */
USTRUCT()
struct FARTIFACT_API FCardManager
{
	GENERATED_BODY()

	FCardManager();
	~FCardManager();

	UPROPERTY(Transient)
	TArray<FCard> cards;

	void Add(const FCard& c);
	void Add(const TArray<FCard>& pcards);
	FCard Remove(uint32_t index);
	FCard Fetch(uint32_t index);
	void Clear();

	///returns exactly the same deck of cards, except hides the ones that owner_id is not meant to be able to see
	FCardManager HideByVisibility(uint64_t powner_id);

	FString Debug();

	template<typename T>
	void Shuffle(T& engine)
	{
		std::shuffle(std::begin(cards), std::end(cards), engine);
	}

	static FCardManager Merge(const FCardManager& c1, const FCardManager& c2);
};

USTRUCT()
struct FOwnedCardManager
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 owner = 0;
	UPROPERTY()
	FCardManager cards;
};

USTRUCT()
struct FARTIFACT_API FBoardState
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<FOwnedCardManager> player_hands;
	UPROPERTY(Transient)
	TArray<FOwnedCardManager> board_states;
	UPROPERTY(Transient)
	TArray<FOwnedCardManager> player_decks;

	void AddPlayerAndDeck(uint64 player_id, const FCardManager& deck);

	FBoardState();
	~FBoardState();

	FString Debug();
};