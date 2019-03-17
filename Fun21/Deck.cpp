#include "Deck.h"
#include "Card.h"
#include "DataTables.h"
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace GEX
{
	namespace {
		const std::map<CardType, CardData> TABLE = initializeCardData();
	}

	Deck::Deck(const TextureManager & textures) : Entity()
	{
		srand(time(0));

		if (_deck.size() != 0)
			_deck.clear();

		for (int i=0;i<3;i++)
			for (int i = 0; i < (int)CardType::END; i++)
				_deck.push_back(new Card(textures, TABLE.at(CardType(i)).face, TABLE.at(CardType(i)).suit, CardType(i)));

		_topOfDeck = _deck.size() - 1;
	}

	bool Deck::isEmpty() const
	{
		return (_topOfDeck == _deck.size());
	}

	Card& Deck::drawCard()
	{
		if (isEmpty())
		{
			shuffle();
			_topOfDeck = 0;
			return *_deck[_topOfDeck++];
		}
		else
			return *_deck[_topOfDeck++];
	}

	void Deck::shuffle()
	{
		std::random_shuffle(_deck.begin(), _deck.end());
	}

	int Deck::getDeckSize()
	{
		return _deck.size();
	}
}
