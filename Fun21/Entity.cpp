/*
*@author: Greg VanKampen
*@file: Entity.cpp
*@description: Entity class that defines the base of what an entity is
*/
#include "Entity.h"
#include <cassert>

GEX::Entity::Entity()
{
	_isAlive = true;
}

void GEX::Entity::setVelocity(sf::Vector2f velocity)
{
	_velocity = velocity;
}

void GEX::Entity::setVelocity(float vx, float vy)
{
	_velocity.x = vx;
	_velocity.y = vy;
}

sf::Vector2f GEX::Entity::getVelocity()
{
	return _velocity;
}

void GEX::Entity::accelerate(sf::Vector2f velocity)
{
	_velocity += velocity;
}

void GEX::Entity::accelerate(float vx, float vy)
{
	_velocity.y += vy;
	_velocity.x += vx;
}

void GEX::Entity::updateLocaton(sf::Vector2f location)
{
	this->setPosition(this->getPosition() + location);
}

void GEX::Entity::updateLocaton(float x, float y)
{
	this->setPosition(this->getPosition().x + y, this->getPosition().y + y);
}


void GEX::Entity::destroy()
{
	_isAlive = false;
}

bool GEX::Entity::isDestroyed() const
{

	return _isAlive == false;
}

void GEX::Entity::remove()
{
	destroy();
}

void GEX::Entity::updateCurrent(sf::Time dt,CommandQueue& commands)
{
	move(_velocity * dt.asSeconds());
}
