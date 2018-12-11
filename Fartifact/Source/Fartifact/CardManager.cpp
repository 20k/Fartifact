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
