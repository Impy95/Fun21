/*
*@author: Greg VanKampen
*@file: SoundsNode.cpp
*@description: A class which contains sounds and its location
*/
#include "SoundNode.h"
#include "SoundPlayer.h"


GEX::SoundNode::SoundNode(SoundPlayer & player)
	:SceneNode(),
	_sounds(player)
{
}

void GEX::SoundNode::playSound(SoundEffectID sound, sf::Vector2f position)
{
	_sounds.play(sound, position);
}

unsigned int GEX::SoundNode::getCategory() const
{
	return Category::SoundEffect;
}
