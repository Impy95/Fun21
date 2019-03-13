#include "Hand.h"

namespace GEX
{
	Hand::Hand()
	{
	}

	void Hand::addCard(Card& card)
	{
		_hand.push_back(&card);
		cards.insert(&card);
	}

	void Hand::removeCard(int n)
	{
		//auto card = &_hand[n];
		//auto it = find_if(_hand.begin(),
		//	_hand.end(),
		//	[=](auto c)
		//{
		//	return c == card;
		//});
		//_hand.erase(it);
		//cards.erase(*card);
	}

	void Hand::clear()
	{
		_hand.clear();
		cards.clear();
	}

	int Hand::getHandTotal()
	{
		int total = 0;
		bool handContainsAce = false;
		for (auto& card : _hand)
		{
			total += card->getCardValue();
			if (!handContainsAce)
				handContainsAce = card->getFace() == Card::Face::Ace ? true : false;
		}

		// if hand total > 21 and hand contain ace
			// change ace to 1 and return new total
		if (total > 21 && handContainsAce)
		{
			total = 0;
			for (auto& card : _hand)
			{
				if (card->getFace() == Card::Face::Ace)
					total += 1;
				else
					total += card->getCardValue();
			}
		}

		return total;
	}
}
