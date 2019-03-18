#include "Card.h"
#include "DataTables.h"

namespace GEX
{
	namespace {
		const std::map<CardType, CardData> TABLE = initializeCardData();
	}

	Card::Card(const TextureManager & textures, Face face, Suit suit, CardType type)
		: Entity()
		, _sprite(textures.get(TABLE.at(type).texture), TABLE.at(type).textureRect)
		, _cardValue(0)
		, _face(face)
		, _suit(suit)
		, _cardType(type)
	{
	}

	void Card::drawcurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
	}

	unsigned int Card::getCategory() const
	{
		return Category::Card;
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
		switch (_face)
		{
		case Face::Two:
			return 2;
		case Face::Three:
			return 3;
		case Face::Four:
			return 4;
		case Face::Five:
			return 5;
		case Face::Six:
			return 6;
		case Face::Seven:
			return 7;
		case Face::Eight:
			return 8;
		case Face::Nine:
			return 9;
		case Face::Ten:
		case Face::Jack:
		case Face::Queen:
			return 10;
		case Face::Ace:
			return 11;
		default:
			return 0; // shouldn't get here
		}
	}

	void Card::updateCurrent(sf::Time dt, CommandQueue & commands)
	{
	}

	void Card::remove()
	{
		Entity::remove();
	}
}
