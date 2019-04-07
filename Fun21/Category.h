/*
*@author: Vaughn Rowse
*@file: Category.h
*@description: Stores entity Categories
*/
#pragma once

namespace Category {
	enum Type
	{
		None = 0,
		Scene = 1 << 0,
		Frog = 1 << 1,
		KillObstacle = 1 << 2,
		PlatformObstacle = 1 << 3,
		Finish = 1 << 4,
		AirSceneLayer = 1 << 5,
		Pickup = 1 << 6,
		ParticleSystem = 1 << 7,
		Boss = 1 << 8,
		SoundEffect = 1 << 9,
		BonusObstacle = 1 << 10,
		Button = 1 << 11,
		Card = 1 << 12,

		Obstacle = KillObstacle || PlatformObstacle

	};
}