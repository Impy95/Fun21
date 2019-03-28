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
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
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
	using CardPtr = std::unique_ptr<Card>;
	class World
	{
	public:
		World(sf::RenderWindow& outputWindow, sf::RenderTarget& outputTarget,SoundPlayer& sounds);
		~World();
		void							update(sf::Time dt);
		void adaptPlayerPosition();
		void							draw();
		CommandQueue&					getCommandQueue();

		sf::FloatRect					getViewBounds() const;
		void							destroyEntitesOutOfView();
		void							addScore(int score);
		void							updateTexts();
		void							hit(Hand* hand);
		void							deal();
		void							split();
		void							playerDouble();
		void							stay();
		void							bet(int amount);
		void							clearBet();
		void							drawPlayerCards();
		void							drawDealerCards();
		bool							isBlackJack(Hand* hand) { return hand->getHandTotal() == 21; }
		bool							isBusted(Hand* hand) { return hand->getHandTotal() > 21; }
		void							dealersTurn();
		bool							handHasAce(Hand* hand);
		void							endRound();
		void							clearAllCards();
	private:
		enum Layer {
			Background = 0,
			LowerField,
			UpperField,
			Cards,
			DealerCards,
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
		bool							isHitButtonClicked();
	private:
		sf::RenderWindow&				_window;
		BloomEffect						_bloomEffect;
		sf::RenderTarget&				_target;
		sf::RenderTexture				_sceneTexture;
		sf::View						_worldview;
		TextureManager					_textures;
		SoundPlayer&					_sounds;
		TextNode*						_scoreText;
		TextNode*						_lifeText;

		sf::Sprite						_backgroundSprite;

		sf::FloatRect					_hitButtonBoundingBox;
		sf::FloatRect					_stayButtonBoundingBox;
		sf::FloatRect					_doubleButtonBoundingBox;
		sf::FloatRect					_splitButtonBoundingBox;
		sf::FloatRect					_bet5ButtonBoundingBox;
		sf::FloatRect					_bet25ButtonBoundingBox;
		sf::FloatRect					_betMaxButtonBoundingBox;
		sf::FloatRect					_dealButtonBoundingBox;
		sf::FloatRect					_clearBetButtonBoundingBox;
		bool							_isMouseButtonDown;
		bool							_isBetting;
		bool							_isPlayersTurn;

		Player*							_player;
		int								_currentBet;
		Deck*							_deck;
		Hand*							_hand;
		Hand*							_dealerHand;
		std::vector<CardPtr*>			_cards;
		TextNode*						_handTotal;
		TextNode*						_dealerHandTotal;
		TextNode*						_currentBetText;
		TextNode*						_remainingMoneyText;

		SceneNode						_sceneGraph;
		std::vector<SceneNode*>			_sceneLayers;
		CommandQueue					_command;
		sf::FloatRect					_worldBounds;
		sf::Vector2f					_spawnPosition;
		float							_scrollSpeeds;
		//Frog*							_player;
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

