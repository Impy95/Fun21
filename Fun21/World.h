/*
*@author: Greg VanKampen & Vaughn Rowse
*@file: World.h
*@description: A controller to handle collisions and events
*/
#pragma once
#include<SFML/System.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "SceneNode.h"
#include "SpriteNode.h"
#include "TextureManager.h"
#include "Frog.h"
#include "CommandQueue.h"
#include <vector>
#include "PostEffect.h"
#include "BloomEffect.h"
#include "SoundPlayer.h"
#include "Obstacle.h"
#include "FontManager.h"
#include <stdlib.h>
#include <time.h>


namespace sf {
	class RenderTarget;
}
namespace GEX {
	class World
	{
	public:
		World(sf::RenderTarget& outputTarget,SoundPlayer& sounds);
		~World();
		void							update(sf::Time dt);
		void adaptPlayerPosition();
		void							draw();
		CommandQueue&					getCommandQueue();

		void							addObstacles(sf::Time dt);
		void							addObstacle(ObstacleType type, float relx, float rely);
		void							spawnObstacles();
		void							addInitalObstacles();
		sf::FloatRect					getViewBounds() const;
		sf::FloatRect					getSpawnerBounds() const;
		bool							hasAlivePlayer() const;
		bool							hasPlayerReachedEnd() const;
		void							destroyEntitesOutOfView();
		void							addScore(int score);
		void							updateScore();
		void							resetPlayer();
		void							killPlayerInRiver();
		void							updatePlayerOnPlatform();
		void							increaseLevel();
		void							initializeFinishSpawnPoints();
	private:
		enum Layer {
			Background = 0,
			LowerField,
			UpperField,
			LayerCount
		};
		struct SpawnPoint {
			SpawnPoint(ObstacleType type, float x, float y)
			:type(type),
			x(x),
			y(y){}
			ObstacleType type;
			float x;
			float y;
		};
	private:
		void							loadTextures();
		void							buildScene();
		void							handleCollisions();
		void							spawnFinishObstacle(sf::Time dt);
	private:
		BloomEffect						_bloomEffect;
		sf::RenderTarget&				_target;
		sf::RenderTexture				_sceneTexture;
		sf::View						_worldview;
		TextureManager					_textures;
		SoundPlayer&					_sounds;
		TextNode*						_scoreText;
		TextNode*						_lifeText;


		SceneNode						_sceneGraph;
		std::vector<SceneNode*>			_sceneLayers;
		CommandQueue					_command;
		sf::FloatRect					_worldBounds;
		sf::Vector2f					_spawnPosition;
		float							_scrollSpeeds;
		Frog*							_player;
		int								_score;
		sf::FloatRect					_riverArea;

		std::vector<SpawnPoint>			_obstacleSpawnPoint;
		std::vector<Obstacle*>			_lilyPads;

		sf::Clock						_clock;
		sf::Clock						_tractorClock;
		sf::Time						_car1SpawnTimer;
		sf::Time						_tractorSpawnTimer;

		//std::vector<sf::Time>			_clocks;
		std::vector<sf::Time>			_spawnTimers;
		std::vector<ObstacleType>		_obstacleTypes;
		std::vector<sf::Time>			_spawnConst;
		bool							_animationPlaying;

		int								_turtle3Counter;
		int								_turtle2Counter;
		int								_crocCounter;
		int								_levelCounter;
		int								_lilypadCounter;
		sf::Time						_currentFinishSpawnTimer;
		bool							_hasFinishObstacle;
		Obstacle*						_finishObstacle;
		std::vector<sf::Vector2f>		_finishObstacleSpawnPoints;
	};


}
