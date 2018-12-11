// copyrite notuc ur a bum

#pragma once

#include "CoreMinimal.h"

#include <vector>
#include <string>

struct Card
{
	static std::vector<std::string> short_names;

	enum class type
	{
		AMERICAN_DOLLAR = 0,
		POUND = 1,
		DONG = 2,
		RUBLE = 3,
		EURO = 4,
		PESO = 5,
		RUPEE = 6,
		BUTTONS = 7,
		CANADIAN_DOLLAR = 8,
		AUSTRALIAN_DOLLARYDOO = 9,
		ZIMBABWEAN_DOLLARS = 10,
		YUAN = 11,
		COUNT,
		NONE
	};

	std::string get_short_name();

	type which = type::NONE;
};

/**
 * Collection of cards, a logical pile of some description
 */
class FARTIFACT_API CardManager
{
public:
	CardManager();
	~CardManager();

	std::vector<Card> cards;

	void Add(const Card& c);
	void Add(const std::vector<Card>& pcards);
	Card Remove(uint32_t index);
	Card Fetch(uint32_t index);
	void Clear();

	static CardManager Merge(const CardManager& c1, const CardManager& c2);

	template<typename T>
	void Shuffle(T& engine)
	{
		std::shuffle(std::begin(cards), std::end(cards), engine);
	}
};
