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
		void							updateTexts();
		void							hit(Hand* hand);
		void							deal();
		void							split();
		void							playerDouble();
		void							stay();
		void							bet(int amount);
		void							clearBet();
		void							initalDrawPlayerCard(Card& card);
		void							drawPlayerCard(sf::Time dt);
		void							initialDrawDealerCard(Card& card);
		void							drawDealerCard(sf::Time dt);
		void							drawSplitHands();
		void							moveCardBack(sf::Time dt);
		bool							isBlackJack(Hand* hand) { return hand->getHandTotal() == 21; }
		bool							isBusted(Hand* hand) { return hand->getHandTotal() > 21; }
		void							dealersTurn();
		bool							handHasAce(Hand* hand);
		void							endRound();
		void							clearAllCards();
		sf::Vector2f					interpolate(const sf::Vector2f& pointA, const sf::Vector2f& pointB, float factor);
		bool							hasMoneyLeft() { return _player->getTotalMoney() > 0; }
		bool							isRoundinProgess() { return _roundInProgress; }
	private:
		enum Layer {
			Background = 0,
			LowerField,
			UpperField,
			Cards,
			DealerCards,
			CardBack,
			ArrowIndicator,
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
	private:
		sf::RenderWindow&				_window;
		sf::RenderTarget&				_target;
		sf::RenderTexture				_sceneTexture;
		sf::View						_worldview;
		TextureManager					_textures;
		SoundPlayer&					_sounds;

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
		bool							_dealing;
		bool							_roundInProgress;
		bool							_hasSplit;
		bool							_firstHandTurn;
		bool							_splitHandTurn;

		SpriteNode*						_cardBack;
		SpriteNode*						_splitHandIndicator;
		std::vector<sf::Clock>			_cardBackClock;
		std::vector<sf::Time>			_cardBackTime;
		std::vector<Card*>				_allCards;
		std::vector<Card*>				_splitCards;
		std::vector<Card*>				_dealerCards;
		std::vector<sf::Clock>			_clocks;
		std::vector<sf::Clock>			_dealerClocks;
		std::vector<sf::Time>			_cardTimers;
		std::vector<sf::Time>			_dealerTimers;
		std::vector<sf::Clock>			_splitClocks;
		std::vector<sf::Time>			_splitTimers;
		std::unique_ptr<Card>			_dealersFirstCard;
		std::vector<Hand*>				_splitHands;

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
		TextNode*						_winningsText;
		TextNode*						_splitHandTotalText;
		TextNode*						_whichHandText;

		SceneNode						_sceneGraph;
		std::vector<SceneNode*>			_sceneLayers;
		CommandQueue					_command;
		sf::FloatRect					_worldBounds;

		sf::Clock						_clock;
	};


}

