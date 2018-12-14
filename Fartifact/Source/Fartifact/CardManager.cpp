// Fill out your copyright notice in the Description page of Project Settings.

#include "CardManager.h"
#include <assert.h>

std::vector<FString> FCard::short_names
{
	"X",
	"$",
	"£",
	"₫",
	"₽",
	"€",
	"₱",
	"₹",
	"B",
	"🥞",
	"🦘",
	"Z$",
	"¥"
	"ERR",
	"ERR"
};

///these two vectors are both for debugging so kill them at some point
std::vector<FString> long_names
{
	"ARMOK",
	"Dollar",
	"Pound",
	"Dong",
	"Ruble",
	"Euro",
	"Peso",
	"Rupee",
	"Buttons",
	"CDollar",
	"ADollary",
	"ZDollar",
	"Yuan",
	"COUNT",
	"NONE"
};

FString FCard::GetShortName()
{
	int itype = (int)which;

	if (itype < 0 || itype >= (int)type::COUNT)
		throw std::runtime_error("Bad type in card get_short_name");

	return short_names[itype];
}

FString FCard::GetLongName()
{
	int itype = (int)which;

	if (itype < 0 || itype >= (int)type::COUNT)
		throw std::runtime_error("Bad type in card get_short_name");

	return long_names[itype];
}

bool FCard::IsOwnedBy(uint64_t puser_id)
{
	return owner_id == puser_id;
}

bool FCard::IsVisibleTo(uint64_t puser_id)
{
	if (visible == (int)visibility::ALL)
		return true;

	if (visible == (int)visibility::NONE)
		return false;

	if (visible == (int)visibility::OWNER)
		return owner_id == puser_id;

	throw std::runtime_error("Unrecognised visibility option");
}

FCardManager::FCardManager()
{
	assert(Card::short_names.size() == (int)Card::type::NONE);
}

FCardManager::~FCardManager()
{
}

void FCardManager::Add(const FCard& c)
{
	cards.Add(c);
}

void FCardManager::Add(const TArray<FCard>& pcards)
{
	for(const FCard& c : pcards)
		cards.Add(c);
}

FCard FCardManager::Remove(uint32_t index)
{
	if (index < 0 || index >= (uint32_t)cards.Num())
		throw std::runtime_error("Index out of bounds in removing card in card manager");

	FCard c = cards[index];

	cards.RemoveAt(index);

	return c;
}

FCard FCardManager::Fetch(uint32_t index)
{
	if (index < 0 || index >= (uint32_t)cards.Num())
		throw std::runtime_error("Index out of bounds in fetching card in card manager");

	return cards[index];
}

void FCardManager::Clear()
{
	cards.Empty();
}

FCardManager FCardManager::HideByVisibility(uint64_t puser_id)
{
	FCardManager ret;

	for (FCard c : cards)
	{
		if (!c.IsVisibleTo(puser_id))
		{
			c.which = (int)FCard::type::UNKNOWN;
		}

		ret.Add(c);
	}

	return ret;
}

FString FCardManager::Debug()
{
	FString accum;

	for (FCard& c : cards)
	{
		accum += c.GetLongName();
	}

	return accum;
}

FCardManager FCardManager::Merge(const FCardManager& c1, const FCardManager& c2)
{
	FCardManager ret;

	ret.Add(c1.cards);
	ret.Add(c2.cards);

	return ret;
}

void FCardMove::MakePass(FBoardState& board_state)
{
	which = (int)type::PASS;
}

void FCardMove::MakeDraw(FBoardState& board_state, uint64 owner_id)
{
	which = (int)type::MOVE;

	card_offset = -1;

	section_source_offset = (int)FBoardState::board_states::DECKS;
	section_dest_offset = (int)FBoardState::board_states::HANDS;

	card_manager_source_offset = -1;
	card_manager_dest_offset = -1;
}

void FCardMove::MakePlay(FBoardState& board_state, uint64 owner_id, int phand_card_offset)
{
	TArray<FOwnedCardManager> own_cards = board_state.GetCardsFor(FBoardState::board_states::BOARD, owner_id);

	which = (int)type::MOVE;

	card_offset = phand_card_offset;

	section_source_offset = (int)FBoardState::board_states::HANDS;
	section_dest_offset = (int)FBoardState::board_states::BOARD;

	card_manager_source_offset = -1;
	card_manager_dest_offset = own_cards.Num();
}

void FBoardState::AddPlayerAndDeck(uint64 player_id, const FCardManager& deck)
{
	players.Add(player_id);

	FOwnedCardManager owned;
	owned.owner = player_id;
	owned.cards = deck;

	for (FCard& c : owned.cards.cards)
	{
		c.owner_id = player_id;
		c.visible = (int)FCard::visibility::NONE;
	}

	FOwnedCardManager dummy_hand;
	dummy_hand.owner = player_id;

	all_cards[(int)board_states::DECKS].owned.Add(owned);
	all_cards[(int)board_states::HANDS].owned.Add(dummy_hand);

	UE_LOG(LogTemp, Warning, TEXT("Deck size %i"), all_cards[(int)board_states::DECKS].owned.Num());
}

FBoardState FBoardState::HideByVisibility(uint64 player_id)
{
	FBoardState ret = *this;

	for (FOwnedCardManager& check : ret.all_cards[(int)board_states::HANDS].owned)
		check.cards = check.cards.HideByVisibility(player_id);

	for (FOwnedCardManager& check : ret.all_cards[(int)board_states::BOARD].owned)
		check.cards = check.cards.HideByVisibility(player_id);

	for (FOwnedCardManager& check : ret.all_cards[(int)board_states::DECKS].owned)
		check.cards = check.cards.HideByVisibility(player_id);

	return ret;
}

TArray<FOwnedCardManager> FBoardState::GetCardsFor(board_states states, uint64 player_id)
{
	TArray<FOwnedCardManager> ret;

	for (auto& i : all_cards[(int)states].owned)
	{
		if (i.owner != player_id)
			continue;

		ret.Add(i);
	}

	return ret;
}

TArray<FOwnedCardManager*> FBoardState::GetCardsForPtr(board_states states, uint64 player_id)
{
	TArray<FOwnedCardManager*> ret;

	for (auto& i : all_cards[(int)states].owned)
	{
		if (i.owner != player_id)
			continue;

		ret.Add(&i);
	}

	return ret;
}


FBoardState::FBoardState()
{
	all_cards.SetNum((int)board_states::COUNT);
}

FBoardState::~FBoardState()
{

}

FString FBoardState::Debug()
{
	FString accum;

	accum = "Hands: ";

	for (FOwnedCardManager& owned : all_cards[(int)board_states::HANDS].owned)
	{
		accum += owned.cards.Debug() + "\n";
	}

	accum += "Board: ";

	for (FOwnedCardManager& board : all_cards[(int)board_states::BOARD].owned)
	{
		accum += board.cards.Debug() + "\n";
	}

	accum += "Decks: ";

	for (FOwnedCardManager& deck : all_cards[(int)board_states::DECKS].owned)
	{
		accum += deck.cards.Debug();
	}

	accum += "Turn: " + FString::FromInt(turn_offset) + "\n";

	return accum;
}