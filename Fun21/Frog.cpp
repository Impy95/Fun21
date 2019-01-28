/*
*@author: Greg VanKampen & Vaughn Rowse
*@file: Frog.cpp
*@description: The entity which is controlled by the player
*/

#include "Frog.h"
#include "DataTables.h"
#include "CommandQueue.h"
#include <string>
#include "Utility.h"
#include <iostream>
using namespace std::placeholders;


namespace GEX {
	namespace {
		const FrogData TABLE = intitalizeFrogData();
	}
	
	Frog::Frog(const TextureManager & textures)
		:Entity(),
		_sprite(textures.get(TABLE.texture),TABLE.textureRect),
		_isMarkedForRemoval(false),
		_travelDistance(0.f),
		_directionIndex(0),
		_highestTileMovement(0),
		_currentTileMovement(0),
		_movementScore(0),
		_lives(3),
		_isOnPlatform(false)
	{
		centerOrigin(_sprite);
		//set up animations
		for (auto a : TABLE.animations)
		{
			_animations[a.first] = a.second;
		}
	}
	//draws the player sprite
	void Frog::drawcurrent(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);	
	}
	//returns frog category
	unsigned int Frog::getCategory() const
	{
		return Category::Frog;
	}

	//gets the area the frog can interact with other entities
	sf::FloatRect Frog::getBoundingBox() const
	{
		sf::FloatRect reducedBoundingBox = _sprite.getGlobalBounds();
		reducedBoundingBox.height = 30;
		reducedBoundingBox.width = 30;
		reducedBoundingBox.left = reducedBoundingBox.left+5;
		reducedBoundingBox.top = reducedBoundingBox.top+5;
		return getWorldTransform().transformRect(reducedBoundingBox);
	}

	void Frog::playLocalSound(CommandQueue & commands, SoundEffectID sound)
	{
		Command playSoundCommand;
		playSoundCommand.category = Category::SoundEffect;
		playSoundCommand.action = derivedAction<SoundNode>(
			std::bind(&SoundNode::playSound, _1, sound, getWorldPosition()));
		commands.push(playSoundCommand);
	}
	
	
	bool Frog::isMarkedForRemoval() const
	{
		return (isDestroyed());
	}

	//adds scores based on furthest position the player has reached
	void Frog::addMovementTotal(int i)
	{
		_currentTileMovement += i;
		if (_currentTileMovement > _highestTileMovement) {
			_highestTileMovement = _currentTileMovement;
			_movementScore += 10;
		}
	}

	//returns cumulative 
	int Frog::getMovementScore()
	{
		int temp = _movementScore;
		_movementScore = 0;
		return temp;
	}

	//resets the players stats and decrements life counter
	void Frog::die()
	{
		if (_state != State::Die) {
			if (_lives > 0) 
			{
				setDirection(State::Die);
				_lives--;
			}
			else {

			}
			_highestTileMovement = 0;
			_currentTileMovement = 0;
		}
	}
	//tests if player is in the dead state
	bool Frog::isDead()
	{
		return _state == State::Die;
	}

	//changes animation based on state
	void Frog::setDirection(State direction)
	{
		_state = direction;
		_animations[_state].restart();
	}

	//returns lives
	int Frog::getLives()
	{
		return _lives;
	}
	//gets if players is on a platform
	bool Frog::isOnPlatform()
	{
		return _isOnPlatform;
	}
	//sets players is on a platform
	void Frog::setIsOnPlatform(bool value)
	{
		_isOnPlatform = value;
	}

	//updates player
	void Frog::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		centerOrigin(_sprite);
		if (isDestroyed()) {
			return;
		}
		//freezes player if in death state
		if (_state != State::Die) {
			Entity::updateCurrent(dt, commands);
		}
		//changes player state after death
		if (isDead() && _animations[_state].isFinished()) {
			_state = State::Up;
		}
		//updates animations
		auto rec = _animations.at(_state).update(dt);
		_sprite.setTextureRect(rec);
		centerOrigin(_sprite);
		
	}

	//removes player entity
	void Frog::remove()
	{
		Entity::remove();
	}

}
