#pragma once
#include "Entity.h"
#include "Card.h"
#include <vector>
#include <memory>

namespace GEX
{
	class Card;

	class Deck : public Entity
	{
	public:
		Deck(const TextureManager& textures);
			
		bool		isEmpty() const;
		Card&		drawCard();
		void		shuffle();
		int			getDeckSize();

	private:
		std::vector<Card*>		_deck;
		int						_topOfDeck = 0;
	};
}

