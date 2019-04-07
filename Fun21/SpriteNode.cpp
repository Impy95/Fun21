/*
*@author: Vaughn Rowse
*@file: SpriteNode.h
*@description: Scenenode for sprites
*/
#include "SpriteNode.h"


namespace GEX {



	SpriteNode::SpriteNode(const sf::Texture & texture):_sprite(texture)
	{
	}

	SpriteNode::SpriteNode(const sf::Texture & texture, const sf::IntRect & textureRect) : _sprite(texture,textureRect)
	{
	}

	SpriteNode::~SpriteNode()
	{
	}

	sf::Sprite SpriteNode::getSprite()
	{
		return _sprite;
	}

	sf::FloatRect SpriteNode::getSpriteGlobalBounds()
	{
		return _sprite.getGlobalBounds();
	}

	sf::FloatRect SpriteNode::getBoundingBox() const
	{
		sf::FloatRect reducedBoundingBox = _sprite.getGlobalBounds();
		reducedBoundingBox.height = 95;
		reducedBoundingBox.width = 290;
		reducedBoundingBox.left = reducedBoundingBox.left + 95;
		reducedBoundingBox.top = reducedBoundingBox.top + 50;
		return getWorldTransform().transformRect(reducedBoundingBox);
	}

	void SpriteNode::drawcurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
		drawBoundingBox(target, states);
	}
}
