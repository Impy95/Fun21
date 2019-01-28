/*
*@author: Greg VanKampen  & Vaughn Rowse
*@file: Frog.h
*@description: The entity which is controlled by the player
*/
#pragma once
#include "Entity.h"
#include "TextureManager.h"
#include "TextNode.h"
#include "Command.h"
#include <SFML\Graphics\Sprite.hpp>
#include "SoundNode.h"
#include "Animation2.h"
#include <functional>
namespace GEX {
	class Frog : public Entity
	{
	public:
		enum class State {
			Up,
			Down,
			Left,
			Right,
			Die
		};

		Frog(const TextureManager& textures);
		virtual void				drawcurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
		unsigned int				getCategory() const override;
		sf::FloatRect				getBoundingBox()const override;
		void						playLocalSound(CommandQueue& commands, SoundEffectID sound);

		bool						isMarkedForRemoval() const override;
		void						addMovementTotal(int i);
		int							getMovementScore();
		void						die();
		bool						isDead();
		void						setDirection(State direction);

		int							getLives();
		bool						isOnPlatform();
		void						setIsOnPlatform(bool value);
	protected:
		void						updateCurrent(sf::Time dt, CommandQueue& commands) override;
	private:
		
		void						remove() override;
		
	private:
		//Animation						explosion;
		sf::Sprite						_sprite;
		bool							_isDead;
		//bool							showExplosion;;
		
		bool							_isMarkedForRemoval;
		float							_travelDistance;
		std::size_t						_directionIndex;
		int								_highestTileMovement;
		int								_currentTileMovement;
		int								_movementScore;
		std::map<State, Animation2>		_animations;
		State							_state;
		int								_lives;
		bool							_isOnPlatform;
	};
}

