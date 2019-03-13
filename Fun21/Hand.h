#pragma once
#include "Entity.h"
#include "Card.h"
#include "Deck.h"
namespace GEX
{
	class Hand : public Entity
	{
	public:
		Hand();
		
		void		addCard(Card card);
		void		removeCard(int n);
		int			handSize() { return _hand.size(); }
		void		clear();
		Card&		getCard(int index) { return _hand[index]; }
		int			getHandTotal();

	private:
		std::vector<Card> _hand;
		std::set<Card> cards;
		std::vector<bool> _drawCard;
	};
}

