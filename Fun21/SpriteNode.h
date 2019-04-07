/*
*@author: Vaughn Rowse
*@file: SpriteNode.h
*@description: Scenenode for sprites
*/
#pragma once
#include "SceneNode.h"
#include <SFML/Graphics.hpp>
namespace GEX {
	class SpriteNode : public SceneNode
	{
	public:
		explicit			SpriteNode(const sf::Texture& texture);
							SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
							~SpriteNode();
		sf::Sprite			getSprite();
		sf::FloatRect		getSpriteGlobalBounds();
		sf::FloatRect		getBoundingBox() const override;
	private:
		virtual void drawcurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Sprite _sprite;
	};
}

