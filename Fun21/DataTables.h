/**
*@author: Greg VanKampen & Vaugn Rowse
*@file: Datatable.cpp
*@description: Storage of attributes for entities
*/
#pragma once
#include "TextureManager.h"
#include "Frog.h"
#include "Pickup.h"
#include "Particle.h"
#include "Animation2.h"
#include <map>
#include "Obstacle.h"

namespace GEX {
	struct Direction {
		Direction(float a, float b)
			:angle(a),
			distance(b) 
		{}

		float					angle;
		float					distance;
	};
	struct FrogData {
		GEX::TextureID				texture;
		sf::IntRect				textureRect;
		std::vector<GEX::Frog::State>	directions;
		std::map<GEX::Frog::State, GEX::Animation2> animations;
	};

	struct ObstacleData
	{
		TextureID								texture;
		sf::IntRect								textureRect;
		sf::Time								spawnTime;
		std::map<Obstacle::State, Animation2>	animations;
		std::vector<Direction>					directions;
		float									speed;
	};

	struct PickupData {
		std::function<void(Frog&)>		action;
		TextureID						texture;
		sf::IntRect						textureRect;
	};

	struct ParticleData {
		sf::Color		color;
		sf::Time		lifetime;
	};
	std::map<Pickup::Type, PickupData>		initalizePickupData();

	std::map<ObstacleType, ObstacleData>	initalizeObstacleData();
	std::map<Particle::Type, ParticleData>	initalizeParticleData();
	//std::map<AircraftType, AircraftData> initalizeAircraftData();
	FrogData								intitalizeFrogData();
	

}
