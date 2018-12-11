// Fill out your copyright notice in the Description page of Project Settings.

#include "CardManager.h"
#include <assert.h>

std::vector<std::string> Card::short_names
{
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

std::string Card::get_short_name()
{
	int itype = (int)which;

	if (itype < 0 || itype >= (int)type::COUNT)
		throw std::runtime_error("Bad type in card get_short_name");

	return short_names[itype];
}

CardManager::CardManager()
{
	assert(Card::short_names.size() == (int)Card::type::NONE);
}

CardManager::~CardManager()
{
}

void CardManager::Add(const Card& c)
{
	cards.push_back(c);
}

void CardManager::Add(const std::vector<Card>& pcards)
{
	for(auto& c : pcards)
		cards.push_back(c);
}

Card CardManager::Remove(uint32_t index)
{
	if (index < 0 || index >= cards.size())
		throw std::runtime_error("Index out of bounds in removing card in card manager");

	Card c = cards[index];

	cards.erase(cards.begin() + index);

	return c;
}

Card CardManager::Fetch(uint32_t index)
{
	if (index < 0 || index >= cards.size())
		throw std::runtime_error("Index out of bounds in fetching card in card manager");

	return cards[index];
}

void CardManager::Clear()
{
	cards.clear();
}

CardManager CardManager::Merge(const CardManager& c1, const CardManager& c2)
{
	CardManager ret;

	ret.Add(c1.cards);
	ret.Add(c2.cards);

	return ret;
}