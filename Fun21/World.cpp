/*
*@author: Vaughn Rowse
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
#include "State.h"
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
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const int MAXBET = 200;

	World::World(sf::RenderWindow& outputWindow, sf::RenderTarget& outputTarget, SoundPlayer& sounds)
		: _window(outputWindow),
		_target(outputTarget),
		_worldview(outputTarget.getDefaultView()),
		_textures(),
		_sceneGraph(),
		_sceneLayers(),
		_worldBounds(0.f, 0.f, _worldview.getSize().x, _worldview.getSize().y),
		_sounds(sounds),
		_isMouseButtonDown(false),
		_isBetting(true),
		_isPlayersTurn(true),
		_dealing(true),
		_hand(new Hand()),
		_dealerHand(new Hand()),
		_handTotal(),
		_dealerHandTotal(),
		_player(new Player(1000)),
		_currentBet(0),
		_cards(),
		_firstHandTurn(false),
		_splitHandTurn(false),
		_splitHands()
	{
		_sceneTexture.create(_target.getSize().x, _target.getSize().y);
		loadTextures();
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

		if ((isBlackJack(_hand) || isBusted(_hand)) && !_hasSplit)
		{
			_isPlayersTurn = false;
			dealersTurn();
		}
		else if ((isBlackJack(_hand) || isBusted(_hand)) && _hasSplit)
		{
			_firstHandTurn = false;
			_splitHandTurn = true;
		}

		if (_splitHands.size() > 0)
		{
			if (isBlackJack(_splitHands[0]) || isBusted(_splitHands[0]))
			{
				_isPlayersTurn = false;
				dealersTurn();
			}
		}

		if (_hasSplit && !_splitHandTurn)
		{
			_firstHandTurn = true;
		}
		//if (isBlackJack(_hand))
		//	endRound();

		if (_firstHandTurn)
			_splitHandIndicator->setPosition(150, 500);
		else if (_splitHandTurn)
			_splitHandIndicator->setPosition(150, 350);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!_isMouseButtonDown)
			{
				sf::Vector2f mouse = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

				sf::FloatRect bounds = _hitButtonBoundingBox;
				//sf::FloatRect bounds = _hitButtonSprite.getGlobalBounds();

				//if (_dealButtonBoundingBox.contains(mouse)) {
				//	std::cout << "Deal Button Pressed \n";
				//	deal();
				//}

				// If the player is currently betting
				// Only these buttons are available to click
				if (_isBetting)
				{
					if (_bet5ButtonBoundingBox.contains(mouse)) 
					{
						_sounds.play(SoundEffectID::ButtonClick);
						bet(5);
					}
					else if (_bet25ButtonBoundingBox.contains(mouse)) 
					{
						_sounds.play(SoundEffectID::ButtonClick);
						bet(25);
					}
					else if (_betMaxButtonBoundingBox.contains(mouse)) 
					{
						_sounds.play(SoundEffectID::ButtonClick);
						bet(MAXBET);
					}
					else if (_dealButtonBoundingBox.contains(mouse)) 
					{
						_sounds.play(SoundEffectID::ButtonClick);
						deal();
					}
					else if (_clearBetButtonBoundingBox.contains(mouse)) 
					{
						_sounds.play(SoundEffectID::ButtonClick);
						clearBet();
					}
				}
				// Else the bet buttons are not available
				// Everything else is
				else
				{
					// If they player has split their hand
					// Hit and Stay have different interactions depending on which hand they are playing
					// Also double is not available after splitting
					if (_hasSplit)
					{
						if (_hitButtonBoundingBox.contains(mouse) && _firstHandTurn)
						{
							_sounds.play(SoundEffectID::ButtonClick);
							hit(_hand);
						}
						else if (_hitButtonBoundingBox.contains(mouse) && _splitHandTurn)
						{
							_sounds.play(SoundEffectID::ButtonClick);
							hit(_splitHands[0]);
						}
						else if (_stayButtonBoundingBox.contains(mouse) && _firstHandTurn)
						{
							_sounds.play(SoundEffectID::ButtonClick);
							stay();
						}
						else if (_stayButtonBoundingBox.contains(mouse) && _splitHandTurn)
						{
							_sounds.play(SoundEffectID::ButtonClick);
							stay();
						}
					}
					// Else every other buttons interaction
					else if (_hitButtonBoundingBox.contains(mouse))
					{
						_sounds.play(SoundEffectID::ButtonClick);
						hit(_hand);
					}
					else if (_stayButtonBoundingBox.contains(mouse))
					{
						_sounds.play(SoundEffectID::ButtonClick);
						stay();
					}
					else if (_doubleButtonBoundingBox.contains(mouse))
					{
						_sounds.play(SoundEffectID::ButtonClick);
						playerDouble();
					}
					else if (_splitButtonBoundingBox.contains(mouse))
					{
						_sounds.play(SoundEffectID::ButtonClick);
						split();
					}
				}
			}

			_isMouseButtonDown = true;
		}
		else
		{
			_isMouseButtonDown = false;
		}

		updateTexts();
		if (_splitHands.size() > 0)
		{
			if (_allCards.size() > 0)
				drawSplitHands();
		}
		else
		{
			if (_allCards.size() > 0)
				drawPlayerCard(dt);
		}
		if (_dealerCards.size() > 0)
			drawDealerCard(dt);
		if (_cardBack != NULL)
			moveCardBack(dt);
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

	//update for player movement
	void World::updateTexts()
	{
		if (_hand->getHandTotal() > 21)
			_handTotal->setText("BUSTED!!!");
		else if (_hand->getHandTotal() == 21)
			_handTotal->setText("BLACKJACK!!!");
		else
			_handTotal->setText("Your Hand: " + std::to_string(_hand->getHandTotal()));

		//if (isBlackJackOrBusted(_hand))
		//	_handTotal->setText("BlackJack or Busted");
		//else
		//	_handTotal->setText("Your Hand: " + std::to_string(_hand->getHandTotal()));

		if (_splitHands.size() > 0)
		{
			_splitHandTotalText->setText("Your Split Hand: " + std::to_string(_splitHands[0]->getHandTotal()));
			if (_firstHandTurn)
				_whichHandText->setText("You are on the bottom hand");
			else if (_splitHandTurn)
				_whichHandText->setText("You are on the top hand");
		}
		else
		{
			_whichHandText->setText("");
			_splitHandTotalText->setText("");
		}
		//_dealerHandTotal->setText("Dealer hand: " + std::to_string(_dealerHand->getHandTotal()));

		_currentBetText->setText("Current Bet: " + std::to_string(_currentBet));
		_remainingMoneyText->setText("Remaining Money: " + std::to_string(_player->getTotalMoney()));
	}

	void World::hit(Hand* hand)
	{
		_sounds.play(SoundEffectID::DealCard);
		if (hand->getHandTotal() < 21)
		{
			_deck->shuffle();
			Card& card = _deck->drawCard();
			//Card* aceTest = new Card(_textures, Card::Face::Ace, Card::Suit::Club, CardType::AceClub);
			//_hand->addCard(*aceTest);
			hand->addCard(card);
			if (_splitHands.size() > 0)
			{
				if (hand == _hand || hand == _splitHands[0])
					initalDrawPlayerCard(card);
				if (hand == _dealerHand)
					initialDrawDealerCard(card);
			}
			else
			{
				if (hand == _hand)
					initalDrawPlayerCard(card);
				if (hand == _dealerHand)
					initialDrawDealerCard(card);
			}
		}
	}

	void World::deal()
	{
		
		if (_currentBet > 0)
		{
			// Play sound effect
			_sounds.play(SoundEffectID::DealCard);

			// Hide winning text
			_winningsText->setText("");

			// Set dealing to true
			_dealing = true;

			// Clear all vectors for the new hands
			_allCards.clear();
			_splitHands.clear();
			_splitCards.clear();
			_cardTimers.clear();
			_clocks.clear();
			_dealerCards.clear();
			_dealerTimers.clear();
			_dealerClocks.clear();
			_cardBackClock.clear();
			_cardBackTime.clear();
			_cardBack = NULL;

			// Remove all current cards from being drawn
			_sceneLayers[Cards]->removeAllChildren();
			_sceneLayers[DealerCards]->removeAllChildren();
			
			// Clear the hands
			_hand->clear();
			_dealerHand->clear();
			
			// Shuffle the deck
			_deck->shuffle();

			// Draw the cards
			Card& card1 = _deck->drawCard();			
			Card& card2 = _deck->drawCard();			
			Card& card3 = _deck->drawCard();			
			Card& card4 = _deck->drawCard();
			
			// Add the cards to their respesctive hands and draw them
			_hand->addCard(card1);
			initalDrawPlayerCard(card1);
			_dealerHand->addCard(card2);
			initialDrawDealerCard(card2);
			_hand->addCard(card3);
			initalDrawPlayerCard(card3);
			_dealerHand->addCard(card4);
			initialDrawDealerCard(card4);

			// Set the betting phase to false
			_isBetting = false;

		}
	}

	void World::split()
	{
		if (_hand->getCard(0).getFace() == _hand->getCard(1).getFace())
		{
			// Clear clocks
			//_clocks.clear();
			// Double bet for extra hand
			_player->betMoney(_currentBet);
			_currentBet *= 2;
			// Create a new hand and add it to the splitHands vector
			Hand* splitHand = new Hand();
			_splitHands.push_back(splitHand);
			_hand->splitHand(*_hand, *_splitHands[0]);
			// Add the card to the split cards vector and remove it from all cards
			_splitCards.push_back(_allCards[1]);
			_allCards.pop_back();
			_hasSplit = true;
			 
			// 200, 500
			// 200, 350
			// Create an arrow indicator for the split hands
			sf::Texture& arrowIndicatorTexture = _textures.get(TextureID::ArrowIndicator);
			std::unique_ptr<SpriteNode> arrowIndicator(new SpriteNode(arrowIndicatorTexture));
			arrowIndicator->setPosition(150, 500);
			arrowIndicator->scale(0.05, 0.05);
			_splitHandIndicator = arrowIndicator.get();
			_sceneLayers[ArrowIndicator]->attachChild(std::move(arrowIndicator));
		}
	}

	void World::playerDouble()
	{
		// Double the bet
		// Deal 1 more card
		// Move to dealers turn
		_player->betMoney(_currentBet);

		_currentBet *= 2;

		hit(_hand);

		dealersTurn();
	}

	void World::stay()
	{
		// If player has split
		// Check if its their first hand, if so move to second hand
		// If second hand, move to dealers turn
		if (_hasSplit)
		{
			if (_firstHandTurn)
			{
				_firstHandTurn = false;
				_splitHandTurn = true;
			}
			else if (_splitHandTurn)
			{
				_splitHandTurn = false;
				dealersTurn();
			}
		}
		else
		{
			// Ends players turn
			_isPlayersTurn = false;
			// Begins dealers turn
			dealersTurn();
		}
	}

	void World::bet(int amount)
	{
		// Begin a new round (bet button will be the first thing clicked
		_roundInProgress = true;	
		// Don't enter if the player is already at max bet
		if (_currentBet < 200)
		{
			// if player does not have enough money for the current bet
			// set the bet to their remaning money
			if (_player->getTotalMoney() - amount < 0)
				amount = _player->getTotalMoney();

			if (_currentBet + amount > 200)
			{
				// If amount puts current bet over 200
				// Take amount over off the current bet
				int amountOver = _currentBet + amount - 200;
				int betAmount = MAXBET - amountOver;
				_player->betMoney(betAmount);
				_currentBet += betAmount;
			}
			else
			{
				_player->betMoney(amount);
				_currentBet += amount;
			}
		}
	}

	// Clear all bets
	void World::clearBet()
	{
		_player->addMoney(_currentBet);
		_currentBet = 0;
	}

	// Draws the cards for the players
	void World::initalDrawPlayerCard(Card & card)
	{
		// If its the split hands turn
		// Cards are drawn in a position above the first hand
		if (_splitHandTurn)
		{
			std::unique_ptr<Card> cardDraw(new Card(_textures, card.getFace(), card.getSuit(), card.getType()));
			cardDraw->setPosition(850, 50);
			cardDraw->setScale(0.3, 0.3);
			_splitCards.push_back(cardDraw.get());
			_sceneLayers[Cards]->attachChild(std::move(cardDraw));
		}
		// Else draw cards in normal positions
		else
		{
			std::unique_ptr<Card> cardDraw(new Card(_textures, card.getFace(), card.getSuit(), card.getType()));
			cardDraw->setPosition(850, 50);
			cardDraw->setScale(0.3, 0.3);
			_allCards.push_back(cardDraw.get());
			_sceneLayers[Cards]->attachChild(std::move(cardDraw));
		}
	}

	// Draws the players cards every frame acting as the animation from the main deck
	// To the players hand
	void World::drawPlayerCard(sf::Time dt)
	{
		float factor = 0.f, speed = .4f;

		sf::Vector2f positionA = sf::Vector2f(850, 50);

		for (int i = 0; i < _allCards.size(); i++)
		{
			sf::Vector2f positionB = sf::Vector2f(200 + (i * 100), 500);

			if (_allCards[i]->getPosition() != positionB)
			{
				if (_clocks.size() != _allCards.size())
				{
					sf::Clock clock;
					_clocks.push_back(clock);
					sf::Time time;
					_cardTimers.push_back(time);
				}
				_cardTimers[i] += _clocks[i].restart();
				factor += _cardTimers[i].asSeconds() * speed;

				_allCards[i]->setPosition(interpolate(positionA, positionB, factor));
			}
		}
	}

	// Draws the split hands
	// Differs from drawPlayerCards because it has to draw both hands 
	void World::drawSplitHands()
	{
		float factor = 0.f, speed = .4f;

		sf::Vector2f positionA = sf::Vector2f(850, 50);

		for (int i = 0; i < _allCards.size(); i++)
		{
			sf::Vector2f positionB = sf::Vector2f(200 + (i * 100), 500);

			if (_allCards[i]->getPosition() != positionB)
			{
				if (_clocks.size() != _allCards.size())
				{
					sf::Clock clock;
					_clocks.push_back(clock);
					sf::Time time;
					_cardTimers.push_back(time);
				}
				_cardTimers[i] += _clocks[i].restart();
				factor += _cardTimers[i].asSeconds() * speed;

				_allCards[i]->setPosition(interpolate(positionA, positionB, factor));
			}
		}
		if (_splitCards.size() > 0)
		{
			for (int i = 0; i < _splitCards.size(); i++)
			{
				sf::Vector2f positionB = sf::Vector2f(200 + (i * 100), 350);

				if (_splitCards[i]->getPosition() != positionB)
				{
					if (_splitClocks.size() != _splitCards.size())
					{
						sf::Clock clock;
						_splitClocks.push_back(clock);
						sf::Time time;
						_splitTimers.push_back(time);
					}
					_splitTimers[i] += _splitClocks[i].restart();
					factor += _splitTimers[i].asSeconds() * speed;

					_splitCards[i]->setPosition(interpolate(positionA, positionB, factor));
				}
			}
		}
	}

	// Initial draw of the dealers cards
	// First card is face down if its the players turn
	void World::initialDrawDealerCard(Card & card)
	{
		if (!_isPlayersTurn)
		{
			std::unique_ptr<Card> dealerCard(new Card(_textures, card.getFace()
				, card.getSuit(), card.getType()));
			dealerCard->setPosition(850, 50);
			dealerCard->setScale(0.3, 0.3);
			_dealerCards.push_back(dealerCard.get());
			_sceneLayers[DealerCards]->attachChild(std::move(dealerCard));
		}
		else
		{
			if (_dealerHand->handSize() == 1)
			{
				std::unique_ptr<Card> dealerCard(new Card(_textures, card.getFace()
					, card.getSuit(), card.getType()));
				dealerCard->setScale(0.3, 0.3);
				dealerCard->setPosition(200, 100);
				_dealerCards.push_back(dealerCard.get());
				_dealersFirstCard = std::move(dealerCard);

				// Draws a card back for the first card to show the card is face down
				sf::Texture& cardBackTexture = _textures.get(TextureID::CardBack);
				std::unique_ptr<SpriteNode> cardBack(new SpriteNode(cardBackTexture));
				cardBack->setPosition(850, 50);
				cardBack->scale(0.2, 0.2);
				_cardBack = cardBack.get();
				_sceneLayers[CardBack]->attachChild(std::move(cardBack));
				//_sceneLayers[DealerCards]->attachChild(std::move(dealerCard));
			}
			else
			{
				// Second card draws as normal
				std::unique_ptr<Card> dealerCard(new Card(_textures, _dealerHand->getCard(1).getFace()
					, _dealerHand->getCard(1).getSuit(), _dealerHand->getCard(1).getType()));
				dealerCard->setPosition(850, 50);
				dealerCard->setScale(0.3, 0.3);
				_dealerCards.push_back(dealerCard.get());
				_sceneLayers[DealerCards]->attachChild(std::move(dealerCard));
			}
		}
	}

	// Constant update of dealers cards
	// To replicate an animation
	void World::drawDealerCard(sf::Time dt)
	{
		float factor = 0.f, speed = .4f;

		sf::Vector2f positionA = sf::Vector2f(850, 50);

		if (_dealing)
		{
			sf::Vector2f positionB = sf::Vector2f(300, 100);
			if (_dealerClocks.size() != 2)
			{
				sf::Clock clock;
				_dealerClocks.push_back(clock);
				_dealerClocks.push_back(clock);
				sf::Time time;
				_dealerTimers.push_back(time);
				_dealerTimers.push_back(time);	
			}
			_dealerTimers[1] += _dealerClocks[1].restart();
			factor += _dealerTimers[1].asSeconds() * speed;

			_dealerCards[1]->setPosition(interpolate(positionA, positionB, factor));
		}
		else
		{
			for (int i = 0; i < _dealerCards.size(); i++)
			{
				sf::Vector2f positionB = sf::Vector2f(200 + (i * 100), 100);

				if (_dealerCards[i]->getPosition() != positionB)
				{
					if (_dealerClocks.size() < _dealerCards.size())
					{
						sf::Clock clock;
						_dealerClocks.push_back(clock);
						sf::Time time;
						_dealerTimers.push_back(time);
					}
					_dealerTimers[i] += _dealerClocks[i].restart();
					factor += _dealerTimers[i].asSeconds() * speed;

					_dealerCards[i]->setPosition(interpolate(positionA, positionB, factor));
				}
			}
		}
	}

	// Same as the other draw cards 
	// Except it is just for the dealers card back
	// So no for loop and positionB is static
	void World::moveCardBack(sf::Time dt)
	{
		float factor = 0.f, speed = .4f;

		sf::Vector2f positionA = sf::Vector2f(850, 50);
		sf::Vector2f positionB = sf::Vector2f(200, 100);

		if (_cardBack != NULL)
		{
			sf::Clock clock;
			_cardBackClock.push_back(clock);
			sf::Time time;
			_cardBackTime.push_back(time);
			_cardBackTime[0] += _cardBackClock[0].restart();
			factor += _cardBackTime[0].asSeconds() * speed;

			_cardBack->setPosition(interpolate(positionA, positionB, factor));
		}
	}

	// The dealers turn
	void World::dealersTurn()
	{
		// Set bools correctly so the game doesn't continuously loop
		_isPlayersTurn = false;
		_dealing = false;
		bool finishedHitting = false;

		// Removes the card back and adds the face down card, face up
		_sceneLayers[CardBack]->removeAllChildren();
		_sceneLayers[DealerCards]->attachChild(std::move(_dealersFirstCard));
		// Logic for when the dealer hits
		// If hand total is less than 17 or a soft 17 (contains an ace), he hits
		// If hand total is greater than 17 or soft 17, he stays
		while (!finishedHitting)
		{
			if (_dealerHand->getHandTotal() < 17 || (_dealerHand->getHandTotal() < 18 && handHasAce(_dealerHand)))
			{
				hit(_dealerHand);
			}
			if ((_dealerHand->getHandTotal() > 16 && !handHasAce(_dealerHand)) 
				|| (_dealerHand->getHandTotal() > 17 && handHasAce(_dealerHand)))
			{
				finishedHitting = true;
				endRound();
			}
		}
	}

	// Checks if a hand has an ace in it
	bool World::handHasAce(Hand * hand)
	{
		for (int i = 0; i < hand->handSize(); i++)
		{
			if (hand->getCard(i).getFace() == Card::Face::Ace)
			{
				return true;
			}
			return false;
		}
	}

	// Ends the current round
	void World::endRound()
	{
		// Draws the split cards to their correct positions so they don't get stuck
		for (int i = 0; i < _splitCards.size(); i++)
		{
			sf::Vector2f positionB = sf::Vector2f(200 + (i * 100), 350);
			_splitCards[i]->setPosition(positionB);
		}
		int winnings = 0;
		// If the hand is split
		// Go through every winning / losing option and set the winning variable correctly
		// Get the winnings and update the texts based off who won
		if (_hasSplit)
		{
			if (isBlackJack(_splitHands[0]))
			{
				winnings += (_currentBet / 2) * 1.5 + (_currentBet / 2);
			}
			else if (isBusted(_splitHands[0]))
			{
				// do nothing
			}
			else if (isBusted(_dealerHand))
			{
				winnings += _currentBet;
			}
			else if (_splitHands[0]->getHandTotal() > _dealerHand->getHandTotal())
			{
				// player wins
				winnings += _currentBet;
			}
			else if (_hand->getHandTotal() == _dealerHand->getHandTotal())
			{
				winnings += _currentBet / 2;
			}

			if (isBlackJack(_hand))
			{
				winnings += (_currentBet / 2) * 1.5 + (_currentBet / 2);
			}
			else if (isBusted(_hand))
			{
				// do nothing
			}
			else if (isBusted(_dealerHand))
			{
				winnings += _currentBet;
			}
			else if (_hand->getHandTotal() > _dealerHand->getHandTotal())
			{
				// player wins
				winnings += _currentBet;
			}
			else if (_hand->getHandTotal() == _dealerHand->getHandTotal())
			{
				winnings += _currentBet / 2;
			}

			if (winnings > 0)
			{
				_winningsText->setText("You Won: " + std::to_string(winnings));
				_player->addMoney(winnings);
			}
			else if (winnings == _currentBet / 2)
			{
				_winningsText->setText("You Pushed");
			}
			else
			{
				_winningsText->setText("Dealer Won");
			}
		}
		// Else just check for the single hand vs the dealers hand
		else
		{
			if (isBlackJack(_hand))
			{
				std::cout << "Blackjack";
				_player->addMoney(_currentBet * 1.5 + _currentBet);
				_winningsText->setText("You Won: " + std::to_string(_currentBet * 1.5 + _currentBet));
			}
			else if (isBusted(_hand))
			{
				std::cout << "Dealer won\n";
				_winningsText->setText("Dealer Won");
			}
			else if (isBusted(_dealerHand))
			{
				std::cout << "Player won\n";
				_player->addMoney(_currentBet * 2);
				_winningsText->setText("You Won: " + std::to_string(_currentBet * 2));
			}
			else if (_dealerHand->getHandTotal() > _hand->getHandTotal() || isBlackJack(_dealerHand))
			{
				// dealer wins
				std::cout << "Dealer won\n";
				_winningsText->setText("Dealer Won");
			}
			else if (_hand->getHandTotal() > _dealerHand->getHandTotal())
			{
				// player wins
				std::cout << "Player won\n";
				_player->addMoney(_currentBet * 2);
				_winningsText->setText("You Won: " + std::to_string(_currentBet * 2));
			}
			else if (_hand->getHandTotal() == _dealerHand->getHandTotal())
			{
				std::cout << "Push";
				_player->addMoney(_currentBet);
				_winningsText->setText("You Pushed");
			}
		}
		// Reset all variables for the new round
		_currentBet = 0;
		_isPlayersTurn = true;
		_isBetting = true;
		//clearAllCards();
		_hand->clear();
		_dealerHand->clear();
		_roundInProgress = false;
		_hasSplit = false;
		_splitHands.clear();
		_splitHandTurn = false;
		_firstHandTurn = false;
	}

	void World::clearAllCards()
	{
		_sceneLayers[DealerCards]->removeAllChildren();
		_sceneLayers[Cards]->removeAllChildren();
	}

	// Moves the cards to their correct position based off the factor given
	sf::Vector2f World::interpolate(const sf::Vector2f & pointA, const sf::Vector2f & pointB, float factor)
	{
		if (factor > 1.f)
			factor = 1.f;

		else if (factor < 0.f)
			factor = 0.f;

		return pointA + (pointB - pointA) * factor;
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
		_textures.load(GEX::TextureID::ClearBetButton, "Media/Textures/clearBet.png");
		_textures.load(GEX::TextureID::CardBack, "Media/Textures/card_back.png");
		_textures.load(GEX::TextureID::ArrowIndicator, "Media/Textures/splitHandIndicator.png");
		
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
		handTotalTxt->setText("Your Hand: " + std::to_string(_hand->getHandTotal()));
		handTotalTxt->setPosition(50, 670);
		handTotalTxt->setSize(50);
		_handTotal = handTotalTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(handTotalTxt)));

		// Connects Split Hand Total display system
		std::unique_ptr<TextNode> splitHandTotalTxt(new TextNode(""));
		splitHandTotalTxt->setText("");
		splitHandTotalTxt->setPosition(50, 720);
		splitHandTotalTxt->setSize(50);
		_splitHandTotalText = splitHandTotalTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(splitHandTotalTxt)));

		// For testing purpose, shows dealers hand total
		//std::unique_ptr<TextNode> dealerHandTotalTxt(new TextNode(""));
		//dealerHandTotalTxt->setText("Dealer Hand: " + std::to_string(_dealerHand->getHandTotal()));
		//dealerHandTotalTxt->setPosition(50, 770);
		//dealerHandTotalTxt->setSize(50);
		//_dealerHandTotal = dealerHandTotalTxt.get();
		//_sceneLayers[UpperField]->attachChild((std::move(dealerHandTotalTxt)));

		// Connects Which Hand display system
		std::unique_ptr<TextNode> whichHandTxt(new TextNode(""));
		whichHandTxt->setText("");
		whichHandTxt->setPosition(300, 770);
		whichHandTxt->setSize(50);
		_whichHandText = whichHandTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(whichHandTxt)));

		// Connects Winning Text display system
		std::unique_ptr<TextNode> winningsTxt(new TextNode(""));
		winningsTxt->setPosition(50, 770);
		winningsTxt->setSize(50);
		_winningsText = winningsTxt.get();
		_sceneLayers[UpperField]->attachChild(std::move(winningsTxt));

		// Connects Current Bet display system
		std::unique_ptr<TextNode> currentBetTxt(new TextNode(""));
		currentBetTxt->setText("Current Bet: " + std::to_string(_currentBet));
		currentBetTxt->setPosition(350, 670);
		currentBetTxt->setSize(50);
		_currentBetText = currentBetTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(currentBetTxt)));

		// Connects Money Left display system
		std::unique_ptr<TextNode> moneyLeftTxt(new TextNode(""));
		moneyLeftTxt->setText("Remaining Money: " + std::to_string(_player->getTotalMoney()));
		moneyLeftTxt->setPosition(650, 670);
		moneyLeftTxt->setSize(50);
		_remainingMoneyText = moneyLeftTxt.get();
		_sceneLayers[UpperField]->attachChild((std::move(moneyLeftTxt)));

		//sets background
		sf::Texture& texture = _textures.get(TextureID::Landscape);
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(false);
		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture));
		backgroundSprite->setPosition(0, 0);
		//backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);
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
		bet5Button->setPosition(150, 625);
		bet5Button->scale(0.4, 0.4);
		_bet5ButtonBoundingBox = bet5Button->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(bet5Button));

		sf::Texture& bet25Texture = _textures.get(TextureID::Bet25Button);
		bet25Texture.setRepeated(false);
		std::unique_ptr<SpriteNode> bet25Button(new SpriteNode(bet25Texture, texturesRect));
		bet25Button->setPosition(275, 625);
		bet25Button->scale(0.4, 0.4);
		_bet25ButtonBoundingBox = bet25Button->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(bet25Button));

		sf::Texture& betMaxTexture = _textures.get(TextureID::BetMaxButton);
		betMaxTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> betMaxButton(new SpriteNode(betMaxTexture, texturesRect));
		betMaxButton->setPosition(400, 625);
		betMaxButton->scale(0.4, 0.4);
		_betMaxButtonBoundingBox = betMaxButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(betMaxButton));

		sf::Texture& dealTexture = _textures.get(TextureID::DealButton);
		dealTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> dealbutton(new SpriteNode(dealTexture, texturesRect));
		dealbutton->setPosition(650, 625);
		dealbutton->scale(0.4, 0.4);
		_dealButtonBoundingBox = dealbutton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(dealbutton));

		sf::Texture& clearBetTexture = _textures.get(TextureID::ClearBetButton);
		dealTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> clearBetButton(new SpriteNode(clearBetTexture, texturesRect));
		clearBetButton->setPosition(525, 625);
		clearBetButton->scale(0.4, 0.4);
		_clearBetButtonBoundingBox = clearBetButton->getBoundingBox();
		_sceneLayers[Background]->attachChild(std::move(clearBetButton));
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
	}
}
