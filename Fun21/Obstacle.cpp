/**
*@author: Greg VanKampen & Vaughn Rowse
*@file: Obstacle.cpp
*@description: Obstacle class which holds the entities which the player interacts with
*/
#include "Obstacle.h"
#include "DataTables.h"
#include <iostream>

namespace GEX
{
	namespace
	{
		const std::map<ObstacleType, ObstacleData> TABLE = initalizeObstacleData();
	}
	//moving object constructor
	Obstacle::Obstacle(ObstacleType type, const GEX::TextureManager& textures, int level)
		: Entity()
		, _type(type)
		, _sprite(textures.get(TABLE.at(type).texture), sf::IntRect(1,1,1,1))
		, _finishFrog(textures.get(TABLE.at(ObstacleType::FrogIcon).texture), TABLE.at(ObstacleType::FrogIcon).textureRect)
		, _isMarkedForRemoval(false)
		, _spawnTimer(TABLE.at(type).spawnTime)
		, _state(State::Idle)
		, travelDistance_(0)
		, directionIndex_(0)
		, _hasFinishFrog(false)
		, _hasCroc(false)
		, _currentTurtleState(0)

	{
		//intalize animations
		for (auto a : TABLE.at(type).animations)
		{
			_animations[a.first] = a.second;
		}
		//initalize turtle states
		if(_type == ObstacleType::Turtle2Diving || _type == ObstacleType::Turtle3Diving)
			initializeTurtleStates();
		if(_type == ObstacleType::LilyPad)
			centerOrigin(_finishFrog);
		//set velocity
		setVelocities(type, level);
	}
	//non moving object constructor
	Obstacle::Obstacle(ObstacleType type, const GEX::TextureManager& textures)
		: Entity()
		, _type(type)
		, _sprite(textures.get(TABLE.at(type).texture), sf::IntRect(1, 1, 1, 1))
		, _finishFrog(textures.get(TABLE.at(ObstacleType::FrogIcon).texture), TABLE.at(ObstacleType::FrogIcon).textureRect)
		, _isMarkedForRemoval(false)
		, _spawnTimer(TABLE.at(type).spawnTime)
		, _state(State::Idle)
		, travelDistance_(0)
		, directionIndex_(0)
		, _hasFinishFrog(false)
		, _hasCroc(false)
		, _currentTurtleState(0)

	{
		for (auto a : TABLE.at(type).animations)
		{
			_animations[a.first] = a.second;
		}
		if (_type == ObstacleType::Turtle2Diving || _type == ObstacleType::Turtle3Diving)
			initializeTurtleStates();
		if (_type == ObstacleType::LilyPad)
			centerOrigin(_finishFrog);

	}

	void Obstacle::drawcurrent(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_sprite, states);
		if (_hasFinishFrog) {
			target.draw(_finishFrog,states);
		}
	}
	//returns category based on obstacle type
	unsigned Obstacle::getCategory() const
	{
		if (_type == ObstacleType::LilyPad)
			return Category::Finish;
		else if (_type == ObstacleType::Log1 || _type == ObstacleType::Log2 || _type == ObstacleType::Turtle2
			|| _type == ObstacleType::Turtle3 || _type == ObstacleType::Turtle3Diving || _type == ObstacleType::Turtle2Diving || _type == ObstacleType::Log2_2)
			return Category::PlatformObstacle;
		else if (_type == ObstacleType::BonusFly)
			return Category::BonusObstacle;
		else
			return Category::KillObstacle;
	}
	//gets the interaction bounding box
	sf::FloatRect Obstacle::getBoundingBox() const
	{
		sf::FloatRect defaultBoundingBox = _sprite.getGlobalBounds();		
		return getWorldTransform().transformRect(defaultBoundingBox);
	}
	//returns spawn timer of obstacle
	sf::Time Obstacle::getSpawnTimer() const
	{
		return _spawnTimer;
	}

	//tests if turltes are active or not
	bool Obstacle::isActive() const
	{
		switch (_state) {
		case State::SwimUnder:
		case State::Dive:
		case State::Surface:
			return false;
			break;
		default:
			return true;
		}
	}

	//Sets lilypad to occupied
	void Obstacle::setHasFinishFrog(bool value)
	{
		_hasFinishFrog = value;
	}
	//tests if lilypad is occupied
	bool Obstacle::hasFinishFrog()
	{
		return _hasFinishFrog;
	}

	//cycles turtle states
	void Obstacle::updateTurtleState()
	{
		if (_type == ObstacleType::Turtle2Diving || _type == ObstacleType::Turtle3Diving) {
			if (_animations.at(_state).isFinished()) {
				
				if (_currentTurtleState >= 3)
					_currentTurtleState = 0;
				else
					++_currentTurtleState;
				
				_state =  _turtleStates[_currentTurtleState];
				_animations.at(_state).restart();
				std::cout << int(_state) << "\n";
			}
		}
	}
	//removal function
	bool Obstacle::isMarkedForRemoval() const
	{
		return isDestroyed();
	}
	//sets turtle state cycle order
	void Obstacle::initializeTurtleStates()
	{
		_state = State::SwimOver;
		_turtleStates.push_back(State::SwimOver);
		_turtleStates.push_back(State::Dive);
		_turtleStates.push_back(State::SwimUnder);
		_turtleStates.push_back(State::Surface);
	}
	//deactives lilypad if a croc is on it
	bool Obstacle::hasCroc()
	{
		return _hasCroc;
	}
	//sets lilypad to inactive
	void Obstacle::setHasCroc(bool value)
	{
		_hasCroc = value;
	}
	//real time updates
	void Obstacle::updateCurrent(sf::Time dt, CommandQueue& commands)
	{
		Entity::updateCurrent(dt, commands);
		updateTurtleState();
			auto rec = _animations.at(_state).update(dt);
			_sprite.setTextureRect(rec);
		centerOrigin(_sprite);
	}
	//intalizes velocities
	void Obstacle::setVelocities(ObstacleType type, int level)
	{
		const float car1Speed = 80.f;
		const float car2Speed = 90.f;
		const float car3Speed = 100.f;
		const float truckSpeed = 110.f;
		const float tractorSpeed = 75.f;
		const float turtle2Speed = 80.f;
		const float turtle2AnimateSpeed = 80.f;
		const float turtle3Speed = 70.f;
		const float turtle3AnimateSpeed = 70.f;
		const float log1Speed = 75.f;
		const float log2Speed = 100.f;
		const float log2_2Speed = 85.f;
		const float gatorSpeed = 85.f;
		const float lilyPadSpeed = 0.f;
		switch (type)
		{
		case ObstacleType::Car1:
			setVelocity(-car1Speed, 0.f);
			break;
		case ObstacleType::Car2:
			setVelocity(-car2Speed, 0.f);
			break;
		case ObstacleType::Car3:
			setVelocity(car3Speed, 0.f);
			break;
		case ObstacleType::Truck:
			setVelocity(-truckSpeed, 0.f);
			break;
		case ObstacleType::Tractor:
			setVelocity(tractorSpeed, 0.f);
			break;
		case ObstacleType::Turtle2:
			setVelocity(-turtle2Speed, 0.f);
			break;
		case ObstacleType::Turtle2Diving:
			setVelocity(-turtle2AnimateSpeed, 0.f);
			break;
		case ObstacleType::Turtle3:
			setVelocity(-turtle3Speed, 0.f);
			break;
		case ObstacleType::Turtle3Diving:
			setVelocity(-turtle3AnimateSpeed, 0.f);
			break;
		case ObstacleType::Log1:
			setVelocity(log1Speed, 0.f);
			break;
		case ObstacleType::Log2:
			setVelocity(log2Speed, 0.f);
			break;
		case ObstacleType::Log2_2:
			setVelocity(log2_2Speed, 0.f);
			break;
		case ObstacleType::Gator:
			setVelocity(gatorSpeed, 0.f);
			break;
		case ObstacleType::LilyPad:
			setVelocity(lilyPadSpeed, 0.f);
			break;
		}
		setVelocity(getVelocity().x + getVelocity().x*(((float)level-1) * 0.50) , 0.f);
	}
	//removes obstacle
	void Obstacle::remove()
	{
		Entity::remove();
	}

}
