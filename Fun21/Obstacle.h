/**
*@author: Greg VanKampen & Vaughn Rowse
*@file: Obstacle.h
*@description: Obstacle class which holds the entities which the player interacts with
*/
#pragma once
#include "Entity.h"
#include "TextureManager.h"
#include "Animation2.h"

namespace GEX {
	// Obstacle Types
	enum class ObstacleType
	{
		Car1,
		Tractor,
		Car2,
		Car3,
		Truck,
		Turtle3,
		Log1,
		Log2,
		Turtle2,
		Log2_2,
		Turtle3Diving,
		Turtle2Diving,
		Gator,
		GatorIcon,
		FrogIcon,
		LilyPad,
		BonusFly,
		COUNT_AT_END
	};


	class Obstacle :public GEX::Entity
	{
	public:
		enum class Direction {
			Up,
			Down,
			Left,
			Right
		};
		enum class State {
			Move,
			Idle,
			Finish,
			Dive,
			Surface,
			SwimUnder,
			SwimOver,
		};
						Obstacle(ObstacleType type, const GEX::TextureManager& textures, int level);
						Obstacle(ObstacleType type, const GEX::TextureManager& textures);
		virtual void	drawcurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		unsigned int	getCategory() const override;
		sf::FloatRect	getBoundingBox() const override;
		sf::Time		getSpawnTimer() const;
		bool			isActive() const;
		void			setHasFinishFrog(bool value);
		bool			hasFinishFrog();
		void			updateTurtleState();

		bool			isMarkedForRemoval() const override;
		void			initializeTurtleStates();
		bool			hasCroc();
		void			setHasCroc(bool value);

	protected:
		void			updateCurrent(sf::Time dt, CommandQueue& commands) override;

	private:
		void			setVelocities(ObstacleType type, int level);

		void			remove() override;

	private:
		ObstacleType					_type;
		sf::Sprite						_sprite;
		bool							_isMarkedForRemoval;
		sf::Time						_spawnTimer;
		State							_state;
		std::map<State, Animation2>		_animations;
		bool							_hasFinishFrog;
		sf::Sprite						_finishFrog;

		float							travelDistance_;
		std::size_t						directionIndex_;
		std::vector<State>				_turtleStates;
		int								_currentTurtleState;
		bool							_hasCroc;
	};
}

