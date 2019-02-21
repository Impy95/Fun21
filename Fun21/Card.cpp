#include "Card.h"
#include "DataTables.h"

namespace GEX
{
	namespace {
		const CardData TABLE = initializeCardData();
	}

	Card::Card(const TextureManager & textures, CardType cardType, Suit suit)
		: Entity()
		, _sprite(textures.get(TABLE.texture), TABLE.textureRect)
		, _cardValue(0)
		, _cardType(cardType)
		, _suit(suit)
	{
	}

	void Card::drawcurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}

	unsigned int Card::getCategory() const
	{
		return 0;
	}

	sf::FloatRect Card::getBoundingBox() const
	{
		return sf::FloatRect();
	}

	void Card::playLocalSound(CommandQueue & commands, SoundEffectID sound)
	{
	}

	bool Card::isMarkedForRemoval() const
	{
		return false;
	}

	int Card::getCardValue()
	{
		return 0;
	}

	void Card::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
	}

	void Card::remove()
	{
	}
}
