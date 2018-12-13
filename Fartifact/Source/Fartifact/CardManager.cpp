// Fill out your copyright notice in the Description page of Project Settings.

#include "CardManager.h"
#include <assert.h>

std::vector<std::string> FCard::short_names
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

std::string FCard::GetShortName()
{
	int itype = (int)which;

	if (itype < 0 || itype >= (int)type::COUNT)
		throw std::runtime_error("Bad type in card get_short_name");

	return short_names[itype];
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
	for(auto& c : pcards)
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

std::string FCardManager::Debug()
{
	std::string accum;

	for (FCard& c : cards)
	{
		accum += c.GetShortName();
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

FGameWorld::FGameWorld()
{

}

FGameWorld::~FGameWorld()
{

}