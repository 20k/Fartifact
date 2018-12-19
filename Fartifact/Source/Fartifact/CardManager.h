// copyrite notuc ur a bum

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include <vector>
#include <string>
#include <stdint.h>
#include <type_traits>

#include "Runtime/Core/Public/Containers/Map.h"

#include "CardManager.generated.h"

///this class is entirely unreal's fault
///I legitimately cannot figure out how to get templates or FVariant or TUnion or the mythical TVariant to work
///with replication. So we get this horrible horrible monstrosity. Also there's no replicating a TMap so that's why this class knows
///its own property_type
USTRUCT()
struct FARTIFACT_API FCardVal
{
	GENERATED_USTRUCT_BODY()

	enum class value_type
	{
		STRING,
		INT,
		DOUBLE,
		COUNT
	};

	UPROPERTY()
	int value_type_idx = (int)value_type::COUNT;

	UPROPERTY()
	FString sdata;
	UPROPERTY()
	int idata = 0;
	UPROPERTY()
	double ddata = 0;

	template<typename T>
	bool holds()
	{
		if (std::is_same_v<T, FString> && value_type_idx == (int)value_type::STRING)
			return true;		
		if (std::is_same_v<T, int> && value_type_idx == (int)value_type::INT)
			return true;		
		if (std::is_same_v<T, double> && value_type_idx == (int)value_type::DOUBLE)
			return true;

		return false;
	}

	template<typename T>
	T& get()
	{
		if (!holds<T>())
			throw std::runtime_error("Bad type access in crappy variant");

		if constexpr (std::is_same_v<T, FString>)
			return sdata
		if constexpr (std::is_same_v<T, int>)
			return idata
		if constexpr (std::is_same_v<T, double>)
			return ddata;
	}

	template<typename T>
	void set(const T& in)
	{
		if constexpr (std::is_same_v<T, FString>)
		{
			sdata = in;
			value_type_idx = (int)value_type::STRING;
		}

		if constexpr (std::is_same_v<T, int>)
		{
			idata = in;
			value_type_idx = (int)value_type::INT;
		}

		if constexpr (std::is_same_v<T, double>)
		{
			ddata = in;
			value_type_idx = (int)value_type::DOUBLE;
		}
	}
};

USTRUCT()
struct FARTIFACT_API FCardState
{
	GENERATED_USTRUCT_BODY()

	enum class property_type
	{
		ATTACK,
		DEFENCE,
		HEALTH,
		DESCRIPTION,
		COUNT
	};

	UPROPERTY()
	int which = (int)property_type::COUNT;

	UPROPERTY()
	FCardVal val;
};

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
	
	UPROPERTY()
	uint64 owner_id = 0;

	UPROPERTY()
	TArray<FCardState> attributes;

	bool IsOwnedBy(uint64 puser_id);
	bool IsVisibleTo(uint64 puser_id);

	//check to see if the card matches the other card by overloading the "==" operator.
	FORCEINLINE bool operator==(const FCard &Other) const
	{
		bool Result = true;
		if (which != Other.which || visible != Other.visible || owner_id != Other.owner_id)
		{
			Result = false;
		}
		return Result;
	}
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
	FCardManager HideByVisibility(uint64 powner_id);

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
	uint64 turn_offset = 0;

	UPROPERTY()
	TArray<uint64> players;

	UPROPERTY()
	TArray<FOwnedCardList> all_cards;

	void AddPlayerAndDeck(uint64 player_id, const FCardManager& deck);

	FBoardState HideByVisibility(uint64 player_id);

	TArray<FOwnedCardManager> GetCardsFor(board_states states, uint64 player_id);
	
	///unsafe!
	TArray<FOwnedCardManager*> GetCardsForPtr(board_states states, uint64 player_id);

	FBoardState();
	~FBoardState();


	FString Debug();
};