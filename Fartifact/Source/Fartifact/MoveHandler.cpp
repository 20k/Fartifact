// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveHandler.h"

MoveHandler::MoveHandler()
{
}

MoveHandler::~MoveHandler()
{
}

FMoveResult MoveHandler::Play(FBoardState in, const FCardMove& to_play, uint64 player_id)
{
	if (to_play.which == (int)FCardMove::type::PASS)
		return in;

	///attacks unimplemented
	if (to_play.which == (int)FCardMove::type::ATTACK)
		return BadMove{ "Unimplemented" };

	TArray<FOwnedCardManager*> my_hand = in.GetCardsForPtr(FBoardState::board_states::HANDS, player_id);
	TArray<FOwnedCardManager*> my_deck = in.GetCardsForPtr(FBoardState::board_states::DECKS, player_id);
	TArray<FOwnedCardManager*> my_board = in.GetCardsForPtr(FBoardState::board_states::BOARD, player_id);

	if (to_play.which == (int)FCardMove::type::MOVE)
	{
		int source_pile = to_play.section_source_offset;
		int dest_pile = to_play.section_dest_offset;

		if (source_pile < 0 || source_pile >= (int)FBoardState::board_states::COUNT)
			return BadMove{ "Bad Source Pile" };

		if (dest_pile < 0 || dest_pile >= (int)FBoardState::board_states::COUNT)
			return BadMove{ "Bad Dest Pile" };

		///can't have a destination pile be a deck
		if (dest_pile == (int)FBoardState::board_states::DECKS)
			return BadMove{ "Dest Pile is Deck" };

		///if we're drawing it cannot be from the board
		if (source_pile == (int)FBoardState::board_states::BOARD)
			return BadMove{ "Source Pile is Board" };

		///if we're drawing from the deck, it must be to a hand
		if (source_pile == (int)FBoardState::board_states::DECKS && dest_pile != (int)FBoardState::board_states::HANDS)
			return BadMove{ "Source Pile is Deck and Dest is not Hand" };

		TArray<FOwnedCardManager>& source = in.all_cards[source_pile].owned;
		TArray<FOwnedCardManager>& dest = in.all_cards[dest_pile].owned;

		int source_cmanager = to_play.card_manager_dest_offset;
		int dest_cmanager = to_play.card_manager_source_offset;

		if (source_pile == (int)FBoardState::board_states::DECKS || source_pile == (int)FBoardState::board_states::HANDS)
			source_cmanager = 0;

		if (my_hand.Num() == 0 || my_deck.Num() == 0)
			return BadMove{ "My hand == 0 or my deck == 0" };

		///always plays to the right at the moment
		if (dest_pile == (int)FBoardState::board_states::BOARD)
		{
			dest_cmanager = my_board.Num();

			FOwnedCardManager fowned;
			fowned.owner = player_id;
			
			in.all_cards[dest_pile].owned.Add(fowned);

			my_board = in.GetCardsForPtr(FBoardState::board_states::BOARD, player_id);
		}

		if(dest_pile == (int)FBoardState::board_states::HANDS)
		{
			dest_cmanager = 0;
		}

		if (source_cmanager < 0 || source_cmanager >= in.GetCardsFor((FBoardState::board_states)source_pile, player_id).Num())
			return BadMove{ "Source cmanager OOB" };

		if (dest_cmanager < 0 || dest_cmanager >= in.GetCardsFor((FBoardState::board_states)dest_pile, player_id).Num())
			return BadMove{ "Dest cmanager OOB" };

		FOwnedCardManager* source_card_manager = in.GetCardsForPtr((FBoardState::board_states)source_pile, player_id)[source_cmanager];
		FOwnedCardManager* dest_card_manager = in.GetCardsForPtr((FBoardState::board_states)dest_pile, player_id)[dest_cmanager];
	
		int source_card_offset = to_play.card_offset;

		///if moving from a deck, source card offset must be -1
		if (source_pile == (int)FBoardState::board_states::DECKS)
		{
			if(source_card_offset != -1)
				return BadMove{ "Source Card Offset != -1" };
		}

		if (source_card_offset < 0)
			source_card_offset = source_card_manager->cards.cards.Num() - 1;

		if (source_card_offset < 0 || source_card_offset >= source_card_manager->cards.cards.Num())
			return BadMove{ "Source Card Offset OOB" };

		FCard actual_card = source_card_manager->cards.cards[source_card_offset];

		if (dest_pile == (int)FBoardState::board_states::HANDS)
		{
			actual_card.visible = (int)FCard::visibility::OWNER;
		}

		if (dest_pile == (int)FBoardState::board_states::BOARD)
		{
			actual_card.visible = (int)FCard::visibility::ALL;
		}

		///unused but might as well
		if (dest_pile == (int)FBoardState::board_states::DECKS)
		{
			actual_card.visible = (int)FCard::visibility::NONE;
		}

		dest_card_manager->cards.Add(actual_card);
		source_card_manager->cards.Remove(source_card_offset);

		return in;
	}

	return BadMove{ "Bad Type" };
}