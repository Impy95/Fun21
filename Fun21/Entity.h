/*
*@author: Greg VanKampen
*@file: Entity.h
*@description: Entity class that defines the base of what an entity is
*/
#pragma once
#include "SceneNode.h"
namespace GEX {

	class Entity : public SceneNode
	{
	public:
		explicit		Entity(); // int hitpoints
		void			setVelocity(sf::Vector2f velocity);
		void			setVelocity(float vx, float vy);
		sf::Vector2f	getVelocity();
		void 			accelerate(sf::Vector2f velocity);
		void			accelerate(float vx, float vy);
		void			updateLocaton(sf::Vector2f location);
		void			updateLocaton(float x, float y);
		void			destroy();
		bool			isDestroyed() const override;
		virtual void			remove();
	protected:
		void	updateCurrent(sf::Time dt,CommandQueue& commands) override;
	private:
		sf::Vector2f	_velocity;
		bool			_isAlive;
		
	};
}

