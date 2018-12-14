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
struct FARTIFACT_API FCard
{
	GENERATED_USTRUCT_BODY()

	static std::vector<FString> short_names;

	UENUM()
	enum class type : uint8
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

	UENUM()
	enum class visibility : uint8
	{
		OWNER, ///eg hand, face down cards on the board owned by someone
		ALL, ///visible to everyone, face up card
		NONE, ///not visible to anyone, face down neutral
	};

	FString GetShortName();
	FString GetLongName();

	UPROPERTY()
	int which = (int)type::NONE;
	UPROPERTY()
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
	GENERATED_USTRUCT_BODY()

	FCardManager();
	~FCardManager();

	UPROPERTY()
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
struct FARTIFACT_API FOwnedCardManager
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	uint64 owner = 0;
	UPROPERTY()
	FCardManager cards;
};

struct FBoardState;

USTRUCT()
struct FARTIFACT_API FCardMove
{
	GENERATED_USTRUCT_BODY()

	enum class type : uint8
	{
		PASS,
		MOVE, ///a card from one pile to another, eg from deck to hand or hand to board
		ATTACK,
	};

	UPROPERTY()
	int which = (int)type::PASS;

	UPROPERTY()
	int section_source_offset = 0;

	UPROPERTY()
	int section_dest_offset = 0;

	UPROPERTY()
	int card_offset = 0;
	
	UPROPERTY()
	int card_manager_source_offset = 0;

	///-1 == don't care
	UPROPERTY()
	int card_manager_dest_offset = -1;

	void MakePass(FBoardState& board_state);

	void MakeDraw(FBoardState& board_state, uint64 owner_id);
	void MakePlay(FBoardState& board_state, uint64 owner_id, int phand_card_offset);
};

USTRUCT()
struct FOwnedCardList
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<FOwnedCardManager> owned;
};

USTRUCT()
struct FARTIFACT_API FBoardState
{
	GENERATED_USTRUCT_BODY()

	enum class board_states
	{
		HANDS = 0,
		BOARD = 1,
		DECKS = 2,
		COUNT = 3,
	};

	UPROPERTY()
	TArray<FOwnedCardList> all_cards;

	void AddPlayerAndDeck(uint64 player_id, const FCardManager& deck);

	FBoardState HideByVisibility(uint64 player_id);

	TArray<FOwnedCardManager> GetCardsFor(board_states states, uint64 player_id);

	FBoardState();
	~FBoardState();

	FString Debug();
};