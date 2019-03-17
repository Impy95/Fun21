/*
*@author: Greg VanKampen & Vaughn Rowse
*@file: World.cpp
*@description: A controller to handle collisions and events
*/
#include "World.h"
#include "Frog.h"
#include "Deck.h"
#include "Hand.h"
#include "Pickup.h"
#include "ParticleNode.h"
#include "Card.h"
#include <SFML\Graphics\RenderTarget.hpp>
#include "SoundNode.h"
#include <iostream>
namespace GEX {

	namespace
	{
		const std::map<ObstacleType, ObstacleData> TABLE = initalizeObstacleData();
	}
	const sf::Time TIMETILLCAR1SPAWN = sf::seconds(1.5f);
	const sf::Time TIMETILLTRACTORSPAWN = sf::seconds(2.0f);


	World::World(sf::RenderWindow& outputWindow, sf::RenderTarget& outputTarget, SoundPlayer& sounds)
		: _window(outputWindow),
		_target(outputTarget),
		_worldview(outputTarget.getDefaultView()),
		_textures(),
		_sceneGraph(),
		_sceneLayers(),
		_worldBounds(0.f, 0.f, _worldview.getSize().x, _worldview.getSize().y),
		_spawnPosition(_worldview.getSize().x / 2.f, _worldview.getSize().y),
		_scrollSpeeds(-50.f),
		_bloomEffect(),
		_sounds(sounds),
		_scoreText(),
		_lifeText(),
		_animationPlaying(false),
		_isMouseButtonDown(false),
		_riverArea(0.f, 75.f, 480.f, 240.f),
		_turtle3Counter(0),
		_turtle2Counter(0),
		_crocCounter(0),
		_levelCounter(1),
		_lilypadCounter(0),
		_currentFinishSpawnTimer(sf::Time::Zero),
		_hasFinishObstacle(false),
		_hand(new Hand()),
		_handTotal(),
		_player(new Player(500)),
		_currentBet(0)
	{
		// initialize clock and time vectors
		const auto obstacleTypeEnumSize = int (ObstacleType::COUNT_AT_END);
		for (int i = 0; i < obstacleTypeEnumSize - 1; i++)
		{
			sf::Time time;
			_spawnTimers.push_back(time);
			_obstacleTypes.push_back(ObstacleType(i));
		}
		_sceneTexture.create(_target.getSize().x, _target.getSize().y);
		loadTextures();
		_score = 0;
		buildScene();
		
		//prep the view
		_worldview.setCenter(_worldview.getSize().x / 2.f, _worldBounds.height - _worldview.getSize().y / 2.f);
		std::cout << "x: " << _worldview.getSize().x << "\ny: " << _worldview.getSize().y;

		//initalize randomizer
		srand(time(NULL));

		_deck = new Deck(_textures);
		//_hand = new Hand();

	}


	World::~World()
	{
	}

	void World::update(sf::Time dt)
	{
		//run all commands in queue
		while (!_command.isEmpty()) {
			_sceneGraph.onCommand(_command.pop(), dt);
		}
		_sceneGraph.update(dt,_command);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!_isMouseButtonDown)
			{
				sf::Vector2f mouse = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

				sf::FloatRect bounds = _hitButtonBoundingBox;
				//sf::FloatRect bounds = _hitButtonSprite.getGlobalBounds();

				if (_hitButtonBoundingBox.contains(mouse))
				{
					std::cout << "Hit Button Pressed\n";
					hit();
				}
				else if (_stayButtonBoundingBox.contains(mouse))
				{
					std::cout << "Stay Button Pressed\n";
					stay();
				}
				else if (_doubleButtonBoundingBox.contains(mouse))
				{
					std::cout << "Double Button Pressed\n";
					playerDouble();
				}
				else if (_splitButtonBoundingBox.contains(mouse))
				{
					std::cout << "Split Button Pressed\n";
					split();
				}
				else if (_bet5ButtonBoundingBox.contains(mouse)) {
					std::cout << "Bet 5 Button Pressed\n";
					bet(5);
				} 
				else if (_bet25ButtonBoundingBox.contains(mouse)) {
					std::cout << "Bet 25 Button Pressed\n";
					bet(25);
				}
				else if (_betMaxButtonBoundingBox.contains(mouse)) {
					std::cout << "Bet Max Button Pressed\n";
					bet(_player->getTotalMoney());
				} 
				else if (_dealButtonBoundingBox.contains(mouse)) {
					std::cout << "Deal Button Pressed \n";
					deal();
				}
				else
				{
					std::cout << "You clicked nothing idiot\n";
				}

				std::cout << "Mouse X: " << mouse.x
					<< "\nMouse Y: " << mouse.y << "\n";
			}

			_isMouseButtonDown = true;
		}
		else
		{
			_isMouseButtonDown = false;
		}

		updateTexts();
		drawPlayerCards();
	}

	//Manages players movement patterns created from world interaction
	void World::adaptPlayerPosition() 
	{
		const float BORDER_DISTANCE_HORIZONTAL = 40.f;
		const float BORDER_DISTANCE_BOTTOM = 20.f;
		const float BORDER_DISTANCE_TOP = 100.f;
		sf::FloatRect viewBounds(_worldview.getCenter() - _worldview.getSize() / 2.f, _worldview.getSize()); //ASK
	}
	
	//render the game
	void World::draw()
	{
		_target.setView(_worldview);
		_target.draw(_sceneGraph);
		
	}

	CommandQueue & World::getCommandQueue()
	{
		return _command;
	}

	//gets display area
	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(_worldview.getCenter() - _worldview.getSize() / 2.f, _worldview.getSize());
	}

	//removes obstacles that leave the view
	void World::destroyEntitesOutOfView()
	{
		Command command;
		command.category = Category::Type::KillObstacle;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt) 
		{
			int i = 1;
			//if (!getSpawnerBounds().intersects(e.getBoundingBox())) {
				//e.remove();
			//}
		});
		_command.push(command);
		command.category = Category::Type::PlatformObstacle;
		_command.push(command);
	}

	//adds to score count
	void World::addScore(int score)
	{
		_score += score;
	}
	//update for player movement
	void World::updateTexts()
	{
		if (_hand->getHandTotal() > 21)
			_handTotal->setText("BUSTED!!!");
		else
			_handTotal->setText("Hand Total: " + std::to_string(_hand->getHandTotal()));

		_currentBetText->setText("Current Bet: " + std::to_string(_currentBet));
		_remainingMoneyText->setText("Remaining Money: " + std::to_string(_player->getTotalMoney()));
	}

	void World::hit()
	{
		if (_hand->getHandTotal() <= 21)
		{
			_deck->shuffle();
			Card& card = _deck->drawCard();
			//Card* aceTest = new Card(_textures, Card::Face::Ace, Card::Suit::Club, CardType::AceClub);
			//_hand->addCard(*aceTest);
			_hand->addCard(card);
			//drawPlayerCard(card);
		}
	}

	void World::deal()
	{
		_hand->clear();
		_deck->shuffle();
		for (int i = 0; i < 2; i++) 
		{
			Card& card = _deck->drawCard();
			_hand->addCard(card);
			//drawPlayerCard(card);
		}

		//std::cout << _deck->getDeckSize();
	}

	void World::split()
	{
	}

	void World::playerDouble()
	{
	}

	void World::stay()
	{
	}

	void World::bet(int amount)
	{
		_player->betMoney(amount);
		_currentBet += amount;
	}

	void World::drawPlayerCards()
	{
		// Testing Aces
		//std::unique_ptr<Card> cardDraw(new Card(_textures, Card::Face::Ace, Card::Suit::Club, CardType::AceClub));
		if (_hand->handSize() > 0)
		{
			for (int i = 0; i < _hand->handSize(); i++)
			{
				std::unique_ptr<Card> cardDraw(new Card(_textures, _hand->getCard(i).getFace(), _hand->getCard(i).getSuit(), _hand->getCard(i).getType()));
				cardDraw->setPosition(200 + (i * 100), 500);
				cardDraw->setScale(0.3, 0.3);
				_sceneLayers[Background]->attachChild(std::move(cardDraw));
			}
		}
		//std::unique_ptr<Card> cardDraw(new Card(_textures, card.getFace(), card.getSuit(), card.getType()));
		//cardDraw->setPosition(100 + (_hand->handSize() * 100), 500);
		//cardDraw->setScale(0.3, 0.3);
		//_sceneLayers[Background]->attachChild(std::move(cardDraw));	
	}

	void World::drawDealerCard(Card & card)
	{
	}

	//loads textures
	void World::loadTextures()
	{
		_textures.load(GEX::TextureID::Landscape, "Media/Textures/fun21newtable.png");
		_textures.load(GEX::TextureID::Cards, "Media/Textures/blankcards_noking.png");
		_textures.load(GEX::TextureID::HitButton, "Media/Textures/Hit.png");
		_textures.load(GEX::TextureID::StayButton, "Media/Textures/Stay.png");
		_textures.load(GEX::TextureID::DoubleButton, "Media/Textures/Double.png");
		_textures.load(GEX::TextureID::SplitButton, "Media/Textures/Split.png");
		_textures.load(GEX::TextureID::Bet5Button, "Media/Textures/bet5.png");
		_textures.load(GEX::TextureID::Bet25Button, "Media/Textures/bet25.png");
		_textures.load(GEX::TextureID::BetMaxButton, "Media/Textures/betmax.png");
		_textures.load(GEX::TextureID::DealButton, "Media/Textures/deal.png");
		_textures.load(GEX::TextureID::Frog, "Media/Textures/Atlas.png");
		_textures.load(GEX::TextureID::Entities, "Media/Textures/Atlas.png");
		_textures.load(GEX::TextureID::LilyPad, "Media/Textures/LilyPad.png");
		
	}

	//inital construction of the world
	void World::buildScene()
	{
		//initalizes layers
		for (int i = 0; i < LayerCount; ++i) {
			auto category = (i == UpperField) ? Category::Type::AirSceneLayer : Category::Type::None;
			SceneNode::Ptr layer(new SceneNode(category));
			_sceneLayers.push_back(layer.get());
			_sceneGraph.attachChild(std::move(layer));
		}
		//Connects Hand Total display system
		std::unique_ptr<TextNode> handTotalTxt(new TextNode(""));
		handTotalTxt->setText("Hand Total: " + std::to_string(_hand->getHandTotal()));
		handTotalTxt->setPosition(100, 0);
		handTotalTxt->setSize(50);
		_handTotal = handTotalTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(handTotalTxt)));

		// Connects Current Bet display system
		std::unique_ptr<TextNode> currentBetTxt(new TextNode(""));
		currentBetTxt->setText("Current Bet: " + std::to_string(_currentBet));
		currentBetTxt->setPosition(104, 50);
		currentBetTxt->setSize(50);
		_currentBetText = currentBetTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(currentBetTxt)));

		// Connects Money Left display system
		std::unique_ptr<TextNode> moneyLeftTxt(new TextNode(""));
		moneyLeftTxt->setText("Remaining Money: " + std::to_string(_player->getTotalMoney()));
		moneyLeftTxt->setPosition(157, 100);
		moneyLeftTxt->setSize(50);
		_remainingMoneyText = moneyLeftTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(moneyLeftTxt)));

		//sets background
		sf::Texture& texture = _textures.get(TextureID::Landscape);
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(false);
		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);
		_sceneLayers[Background]->attachChild(std::move(backgroundSprite));

		/*sf::Texture& hitTexture = _textures.get(TextureID::HitButton);
		sf::IntRect hitRect(_worldBounds);
		hitTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> hitButton(new SpriteNode(hitTexture, hitRect));
		hitButton->setPosition(-25, 500);
		hitButton->scale(0.5, 0.5);
		_sceneLayers[Background]->attachChild(std::move(hitButton));
		_hitButtonGlobalBounds = hitButton->getSpriteGlobalBounds();*/

		//std::unique_ptr<HitButton> hitButtonSprite(new HitButton(_textures));
		//hitButtonSprite->setPosition(-25, 500);
		//hitButtonSprite->scale(0.5, 0.5);
		//_hitButtonSprite = hitButtonSprite->getSprite();
		//_sceneLayers[Background]->attachChild(std::move(hitButtonSprite));

		// Create all buttons
		sf::IntRect texturesRect(_worldBounds);

		sf::Texture& hitTexture = _textures.get(TextureID::HitButton);
		std::unique_ptr<SpriteNode> hitButton(new SpriteNode(hitTexture));
		hitButton->setPosition(-25, 500);
		hitButton->scale(0.5, 0.5);
		_hitButtonBoundingBox = hitButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(hitButton));
		
		sf::Texture& stayTexture = _textures.get(TextureID::StayButton);
		stayTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> stayButton(new SpriteNode(stayTexture, texturesRect));
		stayButton->setPosition(-25, 600);
		stayButton->scale(0.5, 0.5);
		_stayButtonBoundingBox = stayButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(stayButton));

		sf::Texture& doubleTexture = _textures.get(TextureID::DoubleButton);
		doubleTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> doubleButton(new SpriteNode(doubleTexture, texturesRect));
		doubleButton->setPosition(810, 500);
		doubleButton->scale(0.5, 0.5);
		_doubleButtonBoundingBox = doubleButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(doubleButton));

		std::unique_ptr<Deck> deck(new Deck(_textures));

		sf::Texture& splitTexture = _textures.get(TextureID::SplitButton);
		splitTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> splitButton(new SpriteNode(splitTexture, texturesRect));
		splitButton->setPosition(810, 600);
		splitButton->scale(0.5, 0.5);
		_splitButtonBoundingBox = splitButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(splitButton));

		sf::Texture& bet5Texture = _textures.get(TextureID::Bet5Button);
		bet5Texture.setRepeated(false);
		std::unique_ptr<SpriteNode> bet5Button(new SpriteNode(bet5Texture, texturesRect));
		bet5Button->setPosition(225, 625);
		bet5Button->scale(0.4, 0.4);
		_bet5ButtonBoundingBox = bet5Button->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(bet5Button));

		sf::Texture& bet25Texture = _textures.get(TextureID::Bet25Button);
		bet25Texture.setRepeated(false);
		std::unique_ptr<SpriteNode> bet25Button(new SpriteNode(bet25Texture, texturesRect));
		bet25Button->setPosition(350, 625);
		bet25Button->scale(0.4, 0.4);
		_bet25ButtonBoundingBox = bet25Button->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(bet25Button));

		sf::Texture& betMaxTexture = _textures.get(TextureID::BetMaxButton);
		betMaxTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> betMaxButton(new SpriteNode(betMaxTexture, texturesRect));
		betMaxButton->setPosition(475, 625);
		betMaxButton->scale(0.4, 0.4);
		_betMaxButtonBoundingBox = betMaxButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(betMaxButton));

		sf::Texture& dealTexture = _textures.get(TextureID::DealButton);
		dealTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> dealbutton(new SpriteNode(dealTexture, texturesRect));
		dealbutton->setPosition(600, 625);
		dealbutton->scale(0.4, 0.4);
		_dealButtonBoundingBox = dealbutton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(dealbutton));
	}
	//checks collision categories pairs
	bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
	{
		unsigned int category1 = colliders.first->getCategory();
		unsigned int category2 = colliders.second->getCategory();

		if (type1 & category1  && type2 & category2) {
			return true;
		}
		if (type1 & category2  && type2 & category1) {
			std::swap(colliders.first, colliders.second);
			return true;
		}
		return false;
	}

	//handles collisions between player and obstacles
	void World::handleCollisions()
	{
		//build list of colliding pairs of scenenodes
		std::set<SceneNode::Pair> collisionPairs;
		_sceneGraph.checkSceneCollision(_sceneGraph, collisionPairs);
		//_player->setIsOnPlatform(false);
		for (SceneNode::Pair pair : collisionPairs) {
			//if (matchesCategories(pair, Category::Type::Frog, Category::Type::KillObstacle)) {
			//	if(_player->getLives() > 0)
			//		_player->die();
			//}
			if (matchesCategories(pair, Category::Type::Frog, Category::Type::Finish)) {
				auto& finish = static_cast<Obstacle&>(*(pair.second));
				if (!finish.hasFinishFrog() && !finish.hasCroc()) {
					finish.setHasFinishFrog(true);
					addScore(500);
					if (_lilypadCounter < 5)
						_lilypadCounter++;
					// checks if all lily pads have been filled
					if (_lilypadCounter == 5)
					{
						// increases level by resetting all lily pads and increasing level counter
						//increaseLevel();
					}
					//resetPlayer();
				}
				//else {
				//	if (_player->getLives() > 0)
				//		_player->die();
				//}
			}
			if (matchesCategories(pair, Category::Type::Frog, Category::Type::PlatformObstacle))
			{
				auto& platform = static_cast<Obstacle&>(*(pair.second));
				//if(platform.isActive())
				//	_player->setIsOnPlatform(true);
				//else
				//	_player->setIsOnPlatform(false);
			}
			if (matchesCategories(pair, Category::Type::Frog, Category::Type::BonusObstacle)) {
				auto& bonus = static_cast<Obstacle&>(*(pair.second));
				addScore(1000);
				bonus.destroy();
			}
		}
	}
	bool World::isHitButtonClicked()
	{

		return false;
	}
}
