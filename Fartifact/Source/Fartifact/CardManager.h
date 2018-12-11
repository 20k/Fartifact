// Fill out your copyright notice in the Description page of Project Settings.

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
 * 
 */
class FARTIFACT_API CardManager
{
public:
	CardManager();
	~CardManager();
};
