/*
*@author: Greg VanKampen & Vaughn Rowse
*@file: World.cpp
*@description: A controller to handle collisions and events
*/
#include "World.h"
#include"Frog.h"
#include "Pickup.h"
#include "ParticleNode.h"
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
	// car2
	// car3
	// truck

	World::World(sf::RenderTarget& outputTarget, SoundPlayer& sounds) : _target(outputTarget),
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
		_riverArea(0.f, 75.f, 480.f, 240.f),
		_turtle3Counter(0),
		_turtle2Counter(0),
		_crocCounter(0),
		_levelCounter(1),
		_lilypadCounter(0),
		_currentFinishSpawnTimer(sf::Time::Zero),
		_hasFinishObstacle(false)
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
		//initalize possible spawn points
		//initializeFinishSpawnPoints();

		//initalize randomizer
		srand(time(NULL));

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

		//std::cout << "Test\n";

		//handle special events
		//handleCollisions();
		//_sceneGraph.removeWrecks();
		//killPlayerInRiver();
		_sceneGraph.update(dt,_command);
		//adaptPlayerPosition();
		//updateScore();
		//handle spawning
		//addObstacles(dt);
		//spawnObstacles();
		//spawnFinishObstacle(dt);
		//tests player death
		//if (_player->isDead())
		//	_animationPlaying = true;
		//if (!_player->isDead() && _animationPlaying== true) {
		//	_animationPlaying = false;
		//	resetPlayer();
		//}

		//world events
		//destroyEntitesOutOfView();
	}

	//Manages players movement patterns created from world interaction
	void World::adaptPlayerPosition() {
		//if (_player->isOnPlatform())
		//{
		//	updatePlayerOnPlatform();
		//	// move with platform
		//}
		//else
		//{
		//	_player->setVelocity(0.f, 0.f);
		//}

		//keep player in bounds
		const float BORDER_DISTANCE_HORIZONTAL = 40.f;
		const float BORDER_DISTANCE_BOTTOM = 20.f;
		const float BORDER_DISTANCE_TOP = 100.f;
		sf::FloatRect viewBounds(_worldview.getCenter() - _worldview.getSize() / 2.f, _worldview.getSize()); //ASK
		//sf::Vector2f position = _player->getPosition();
		//position.x = std::max(position.x, viewBounds.left + BORDER_DISTANCE_HORIZONTAL);
		//position.x = std::min(position.x, viewBounds.left + viewBounds.width - BORDER_DISTANCE_HORIZONTAL);
		//position.y = std::max(position.y, viewBounds.top + BORDER_DISTANCE_TOP);
		//position.y = std::min(position.y, viewBounds.top + viewBounds.height - BORDER_DISTANCE_BOTTOM);
		//_player->setPosition(position);
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

	//spawns obstacles on a timer
	void World::addObstacles(sf::Time dt)
	{
		const auto obstacleTypeEnumSize = int(ObstacleType::COUNT_AT_END);
		for (int i = 0; i < 10; i++)
		{
			_spawnTimers[i] += dt;
			sf::Time ObstacleSpawnTimers = TABLE.at(_obstacleTypes[i]).spawnTime * ((float)_levelCounter-1 * 0.5f + 1.f);
			while (_spawnTimers[i] > ObstacleSpawnTimers)
			{
				// if obstacle is a log that cannot be a croc
				if (_obstacleTypes[i] == ObstacleType::Log1 || _obstacleTypes[i] == ObstacleType::Log2)
				{
					addObstacle(_obstacleTypes[i], -250.f, (40.f * (float(i) + 1.f) + 60.f));
				}
				// if obstacle is a log that can be a croc
				else if (_obstacleTypes[i] == ObstacleType::Log2_2)
				{
					// if it is past level 1
					if (_levelCounter > 1)
					{
						// if its the crocs turn to spawn, spawn it and set counter to 0
						if (_crocCounter == 3)
						{
							addObstacle(ObstacleType::Gator, -250.f, (40.f * (float(i) + 1.f) + 60.f));
							_crocCounter = 0;
						}
						// else spawn normal log and increase counter
						else
						{
							addObstacle(_obstacleTypes[i], -250.f, (40.f * (float(i) + 1.f) + 60.f));
							_crocCounter++;
						}
					}
					// else spawn normal log
					else
						addObstacle(_obstacleTypes[i], -250.f, (40.f * (float(i) + 1.f) + 60.f));
				}
				// if its 3 turtles
				else if (_obstacleTypes[i] == ObstacleType::Turtle3)
				{
					// if its the animated turtles turn to spawn, spawn it and set counter to 0
					if (_turtle3Counter == 2)
					{
						addObstacle(ObstacleType::Turtle3Diving, 250.f, (40.f * (float(i) + 1.f) + 60.f));
						_turtle3Counter = 0;
					}
					// else spawn normal turtle and increase counter
					else
					{
						addObstacle(_obstacleTypes[i], 250.f, (40.f * (float(i) + 1.f) + 60.f));
						_turtle3Counter++;
					}
				}
				// if it is 2 turtles
				else if (_obstacleTypes[i] == ObstacleType::Turtle2)
				{
					// if its the animated turtles turn to spawn, spawn it and set counter to 0
					if (_turtle2Counter == 2)
					{
						addObstacle(ObstacleType::Turtle2Diving, 250.f, (40.f * (float(i) + 1.f) + 60.f));
						_turtle2Counter = 0;
					}
					// else spawn normal turtle and increase counter
					else
					{
						addObstacle(_obstacleTypes[i], 250.f, (40.f * (float(i) + 1.f) + 60.f));
						_turtle2Counter++;
					}
				}
				// else spawn cars going right to left
				else if (i % 2 == 0)
					addObstacle(_obstacleTypes[i], 250.f, (40.f * (float(i) + 1.f) + 20.f));
				// spawn cars going left to right
				else
					addObstacle(_obstacleTypes[i], -250.f, (40.f * (float(i) + 1.f) + 20.f));
				_spawnTimers[i] -= TABLE.at(_obstacleTypes[i]).spawnTime;
			}
			
		}
		
		std::sort(_obstacleSpawnPoint.begin(), _obstacleSpawnPoint.end(), [](SpawnPoint lhs, SpawnPoint rhs) {
			return lhs.y < rhs.y;
		});


	}
	//add a single obstacle
	void World::addObstacle(ObstacleType type, float relx, float rely)
	{
		SpawnPoint spawnPoint(type, _spawnPosition.x + relx, _spawnPosition.y - rely);
		_obstacleSpawnPoint.push_back(spawnPoint);
	}


	void World::spawnObstacles()
	{
		while (!_obstacleSpawnPoint.empty() &&
			_obstacleSpawnPoint.back().y > getSpawnerBounds().left)
		{
			auto spawnPoint = _obstacleSpawnPoint.back();
			std::unique_ptr<Obstacle> enemy(new Obstacle(spawnPoint.type, _textures,_levelCounter));
			enemy->setPosition(spawnPoint.x, spawnPoint.y);
			_sceneLayers[LowerField]->attachChild(std::move(enemy));
			_obstacleSpawnPoint.pop_back();
		}
	}

	//spawns static inital entities
	void World::addInitalObstacles()
	{
		//lilypads
		std::unique_ptr<Obstacle> lilyPad1(new Obstacle(ObstacleType::LilyPad, _textures));
		lilyPad1->setPosition(_spawnPosition.x + 0, _spawnPosition.y - 500);
		_lilyPads.push_back(lilyPad1.get());
		_sceneLayers[LowerField]->attachChild(std::move(lilyPad1));

		std::unique_ptr<Obstacle> lilyPad2(new Obstacle(ObstacleType::LilyPad, _textures));
		lilyPad2->setPosition(_spawnPosition.x - 100, _spawnPosition.y - 500);
		_lilyPads.push_back(lilyPad2.get());
		_sceneLayers[LowerField]->attachChild(std::move(lilyPad2));

		std::unique_ptr<Obstacle> lilyPad3(new Obstacle(ObstacleType::LilyPad, _textures));
		lilyPad3->setPosition(_spawnPosition.x - 200, _spawnPosition.y - 500);
		_lilyPads.push_back(lilyPad3.get());
		_sceneLayers[LowerField]->attachChild(std::move(lilyPad3));

		std::unique_ptr<Obstacle> lilyPad4(new Obstacle(ObstacleType::LilyPad, _textures));
		lilyPad4->setPosition(_spawnPosition.x + 100, _spawnPosition.y - 500);
		_lilyPads.push_back(lilyPad4.get());
		_sceneLayers[LowerField]->attachChild(std::move(lilyPad4));

		std::unique_ptr<Obstacle> lilyPad5(new Obstacle(ObstacleType::LilyPad, _textures));
		lilyPad5->setPosition(_spawnPosition.x + 200, _spawnPosition.y - 500);
		_lilyPads.push_back(lilyPad5.get());
		_sceneLayers[LowerField]->attachChild(std::move(lilyPad5));


		//opening obstacle spawns
		for (int i = 0; i < 10; i++)
		{
			if (_obstacleTypes[i] == ObstacleType::Log1 || _obstacleTypes[i] == ObstacleType::Log2
				|| _obstacleTypes[i] == ObstacleType::Log2_2)
				addObstacle(_obstacleTypes[i], -150.f, (40.f * (float(i) + 1.f) + 60.f));
			else if (_obstacleTypes[i] == ObstacleType::Turtle3 || _obstacleTypes[i] == ObstacleType::Turtle2
				|| _obstacleTypes[i] == ObstacleType::Turtle2Diving || _obstacleTypes[i] == ObstacleType::Turtle3Diving)
				addObstacle(_obstacleTypes[i], 150.f, (40.f * (float(i) + 1.f) + 60.f));
			else if (i % 2 == 0)
				addObstacle(_obstacleTypes[i], 100.f, (40.f * (float(i) + 1.f) + 20.f));
			else
				addObstacle(_obstacleTypes[i], -100.f, (40.f * (float(i) + 1.f) + 20.f));
		}
	}

	//gets display area
	sf::FloatRect World::getViewBounds() const
	{
		return sf::FloatRect(_worldview.getCenter() - _worldview.getSize() / 2.f, _worldview.getSize());
	}

	//gets removal area
	sf::FloatRect World::getSpawnerBounds() const
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.left -= 100.f;
		bounds.width += 200.f;
		bounds.top -= 100.f;
		bounds.height += 200.f;
		return bounds;
	}

	//check if player is currently active
	bool World::hasAlivePlayer() const
	{
		//return _player->getLives() > 0 || _player->isDead();
		return true;
	}

	//unused
	bool World::hasPlayerReachedEnd() const
	{
		return false;
	}
	//removes obstacles that leave the view
	void World::destroyEntitesOutOfView()
	{
		Command command;
		command.category = Category::Type::KillObstacle;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt) 
		{
			int i = 1;
			if (!getSpawnerBounds().intersects(e.getBoundingBox())) {
				e.remove();
			}
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
	void World::updateScore()
	{
		//addScore(_player->getMovementScore());
		_scoreText->setText("Score:" + std::to_string(_score));
		//_lifeText->setText("Lives:" + std::to_string(_player->getLives()));
	}
	//resets player position
	void World::resetPlayer()
	{
		//_player->setPosition(_spawnPosition);
	}
	//kills player if they are within the river area
	void World::killPlayerInRiver()
	{
			//if (_riverArea.intersects(_player->getBoundingBox()) && !_player->isOnPlatform()) {
			//	_player->die();
			//}

	}

	void World::updatePlayerOnPlatform()
	{
		Command command;
		command.category = Category::Type::PlatformObstacle;
		command.action = derivedAction<Entity>([this](Entity& e, sf::Time dt)
		{
			int i = 1;
			//if (e.getBoundingBox().intersects(_player->getBoundingBox())) {
			//	_player->setVelocity(e.getVelocity());
			//}
			//else if (!e.getBoundingBox().intersects(_player->getBoundingBox()))
			//{
			//	_player->setVelocity(0.f, 0.f);
			//}
		});
		_command.push(command);
	}

	//increases the current level counter
	void World::increaseLevel()
	{
		_lilypadCounter = 0;
		_levelCounter++;
		for (auto lp : _lilyPads)
		{
			lp->setHasFinishFrog(false);
		}
	}
	//sets locations for lilypad obstacles
	void World::initializeFinishSpawnPoints()
	{
		_finishObstacleSpawnPoints.push_back(sf::Vector2f(_spawnPosition.x - 0, _spawnPosition.y - 500));
		_finishObstacleSpawnPoints.push_back(sf::Vector2f(_spawnPosition.x - 100, _spawnPosition.y - 500));
		_finishObstacleSpawnPoints.push_back(sf::Vector2f(_spawnPosition.x - 200, _spawnPosition.y - 500));
		_finishObstacleSpawnPoints.push_back(sf::Vector2f(_spawnPosition.x + 100, _spawnPosition.y - 500));
		_finishObstacleSpawnPoints.push_back(sf::Vector2f(_spawnPosition.x + 200, _spawnPosition.y - 500));
	}

	//loads textures
	void World::loadTextures()
	{
		_textures.load(GEX::TextureID::Landscape, "Media/Textures/Fun21Table.png");
		_textures.load(GEX::TextureID::Buttons, "Media/Textures/Hit.png");
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
		//Connects Score display system
		//std::unique_ptr<TextNode> scoreTxt(new TextNode(""));
		//scoreTxt->setText("Score: " + std::to_string(_score));
		//scoreTxt->setPosition(100, 0);
		//scoreTxt->setSize(50);
		//_scoreText = scoreTxt.get();
		//_sceneLayers[UpperField]->attachChild((std::move(scoreTxt)));

		////Connects life display system
		//std::unique_ptr<TextNode> lifeTxt(new TextNode(""));
		//lifeTxt->setText("Lives: ");
		//lifeTxt->setPosition(300, 0);
		//lifeTxt->setSize(50);
		//_lifeText = lifeTxt.get();
		//_sceneLayers[UpperField]->attachChild((std::move(lifeTxt)));
		//
		//sets background
		sf::Texture& texture = _textures.get(TextureID::Landscape);
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(false);
		std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
		backgroundSprite->setPosition(_worldBounds.left, _worldBounds.top);
		_sceneLayers[Background]->attachChild(std::move(backgroundSprite));

		sf::Texture& hitTexture = _textures.get(TextureID::Buttons);
		sf::IntRect hitRect(_worldBounds);
		hitTexture.setRepeated(false);
		std::unique_ptr<SpriteNode> hitButton(new SpriteNode(hitTexture, hitRect));
		hitButton->setPosition(400, 400);
		_sceneLayers[Background]->attachChild(std::move(hitButton));

		//add player aircraft & game objects
		//std::unique_ptr<Frog> playerEntity(new Frog(_textures));
		//playerEntity->setPosition(_spawnPosition);
		//_player = playerEntity.get();
		//_sceneLayers[UpperField]->attachChild(std::move(playerEntity));

		//Obstacles
		//addInitalObstacles();
		//addObstacles();
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
						increaseLevel();
					}
					resetPlayer();
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
	//spawns crocodiles and  flyes on a timer
	void World::spawnFinishObstacle(sf::Time dt)
	{
		_currentFinishSpawnTimer += dt;
		sf::Time itemSpawnTime = sf::seconds(5);
		sf::Time itemDeSpawnTime = sf::seconds(15);
		

		if (_currentFinishSpawnTimer > itemSpawnTime && !_hasFinishObstacle) {
			//randomize obstacle & position
			ObstacleType type = ((rand() % 2 == 0) ? ObstacleType::BonusFly : ObstacleType::GatorIcon);
			int lilypadIndex = rand() % 5;
			sf::Vector2f randomSpawn = _finishObstacleSpawnPoints[lilypadIndex];
			if (type == ObstacleType::GatorIcon)
				_lilyPads[lilypadIndex]->setHasCroc(true);
			
			//spawn obstacle
			std::unique_ptr<Obstacle> obstacle(new Obstacle(type, _textures));
			obstacle->setPosition(randomSpawn);
			_finishObstacle = obstacle.get();
			_sceneLayers[LowerField]->attachChild(std::move(obstacle));
			
			_hasFinishObstacle = true;
		}
		if (_currentFinishSpawnTimer > itemDeSpawnTime && _hasFinishObstacle) {
			_finishObstacle->destroy();

			_hasFinishObstacle = false;
			_currentFinishSpawnTimer = sf::Time::Zero;
			for (int i = 0; i < 5; i++) {
				_lilyPads[i]->setHasCroc(false);
			}
		}

	}

}
