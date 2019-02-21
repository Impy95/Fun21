#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
namespace GEX
{
	class TextureManager;

	class HitButton : public Entity
	{
	public:
								HitButton(const TextureManager& textures);

		unsigned int			getCategory() const override;
		sf::FloatRect			getBoundingBox()const override;
		sf::FloatRect			getSpriteGlobalBounds() const;
		sf::Sprite				getSprite() const;

	private:
		void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		sf::Sprite				sprite_;
	};
}

