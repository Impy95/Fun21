#include "HitButton.h"
#include "DataTables.h"

namespace
{
	const GEX::HitButtonData TABLE = GEX::initializeHitButtonData();
}

GEX::HitButton::HitButton(const TextureManager & textures)
	: Entity()
	, sprite_(textures.get(TABLE.texture), TABLE.textureRect)
{
}

unsigned int GEX::HitButton::getCategory() const
{
	return 0;
}

sf::FloatRect GEX::HitButton::getBoundingBox() const
{
	return getWorldTransform().transformRect(sprite_.getGlobalBounds());
}

sf::FloatRect GEX::HitButton::getSpriteGlobalBounds() const
{
	return sprite_.getGlobalBounds();
}

sf::Sprite GEX::HitButton::getSprite() const
{
	return sprite_;
}

void GEX::HitButton::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite_, states);
}
