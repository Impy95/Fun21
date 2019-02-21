#pragma once
#include "Entity.h"
#include "TextureManager.h"
#include "TextNode.h"
#include "Command.h"
#include "State.h"
#include <SFML\Graphics\Sprite.hpp>
#include "SoundNode.h"
#include "Animation2.h"
#include <functional>
namespace GEX
{
	class Card : public Entity
	{
	public:
		enum class CardType
		{
			Ace,
			Two,
			Three,
			Four,
			Five,
			Six,
			Seven,
			Eight,
			Nine,
			Ten,
			Jack,
			Queen
		};

		enum class Suit
		{
			Spade,
			Diamond,
			Heart,
			Club
		};
									Card(const TextureManager& textures, CardType cardType, Suit suit);
		virtual void				drawcurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		unsigned int				getCategory() const override;
		sf::FloatRect				getBoundingBox() const override;
		void						playLocalSound(CommandQueue& commands, SoundEffectID sound);

		bool						isMarkedForRemoval() const override;
		int							getCardValue();
	protected:
		void						updateCurrent(sf::Time dt, CommandQueue& commands) override;
	private:

		void						remove() override;

	private:
		sf::Sprite					_sprite;
		bool						_isMarkedForRemoval;
		int							_cardValue;
		CardType					_cardType;
		Suit						_suit;
	};
}

