// Fill out your copyright notice in the Description page of Project Settings.

#include "CardManagerActor.h"
#include <assert.h>

std::vector<std::string> ACardActor::short_names
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

ACardActor::ACardActor()
{

}

std::string ACardActor::GetShortName()
{
	int itype = (int)which;

	if (itype < 0 || itype >= (int)type::COUNT)
		throw std::runtime_error("Bad type in card get_short_name");

	return short_names[itype];
}

bool ACardActor::IsOwnedBy(uint64_t puser_id)
{
	return owner_id == puser_id;
}

bool ACardActor::IsVisibleTo(uint64_t puser_id)
{
	if (visible == visibility::ALL)
		return true;

	if (visible == visibility::NONE)
		return false;

	if (visible == visibility::OWNER)
		return owner_id == puser_id;

	throw std::runtime_error("Unrecognised visibility option");
}


// Sets default values
ACardManagerActor::ACardManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	assert(ACardActor::short_names.size() == (int)ACardActor::type::NONE);
}

ACardManagerActor::~ACardManagerActor()
{
}

// Called when the game starts or when spawned
void ACardManagerActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACardManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACardManagerActor::Add(ACardActor* c)
{
	cards.push_back(c);
}

void ACardManagerActor::Add(std::vector<ACardActor*>& pcards)
{
	for (auto& c : pcards)
		cards.push_back(c);
}

ACardActor* ACardManagerActor::Remove(uint32_t index)
{
	if (index < 0 || index >= cards.size())
		throw std::runtime_error("Index out of bounds in removing card in card manager");

	ACardActor* c = cards[index];

	cards.erase(cards.begin() + index);

	return c;
}

ACardActor* ACardManagerActor::Fetch(uint32_t index)
{
	if (index < 0 || index >= cards.size())
		throw std::runtime_error("Index out of bounds in fetching card in card manager");

	return cards[index];
}

void ACardManagerActor::Clear()
{
	cards.clear();
}

/*ACardManagerActor ACardManagerActor::HideByVisibility(uint64_t puser_id)
{
	ACardManagerActor ret;

	for (Card c : cards)
	{
		if (!c.IsVisibleTo(puser_id))
		{
			c.which = Card::type::UNKNOWN;
		}

		ret.Add(c);
	}

	return ret;
}*/

std::string ACardManagerActor::Debug()
{
	std::string accum;

	for (ACardActor* c : cards)
	{
		accum += c->GetShortName();
	}

	return accum;
}

/*CardManager ACardManagerActor::Merge(const CardManager& c1, const CardManager& c2)
{
	CardManager ret;

	ret.Add(c1.cards);
	ret.Add(c2.cards);

	return ret;
}*/