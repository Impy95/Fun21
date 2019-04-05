/**
*@author: Greg VanKampen
*@file: GameState.cpp
*@description: State when the game plays in
*/
#include "Gamestate.h"


	Gamestate::Gamestate(GEX::StateStack & stack, Context context)
		:State(stack,context),
		_world(*context.window, *context.window, *context.sound),
		player(*context.player)
	{
		context.music->play(GEX::MusicID::GameTheme);
	}
	void Gamestate::draw()
	{
		_world.draw();
	}
	bool Gamestate::update(sf::Time dt)
	{
		/*if (!_world.hasAlivePlayer()) {
			player.setMissionStatus(GEX::MissionStatus::MissionFailure);
			requestStackPush(GEX::StateID::GameOver);
		}
		else if (_world.hasPlayerReachedEnd()) {
			player.setMissionStatus(GEX::MissionStatus::MissionSuccess);
			requestStackPush(GEX::StateID::GameOver);
		}*/
		if (!_world.hasMoneyLeft() && !_world.isRoundinProgess())
		{
			player.setMissionStatus(GEX::MissionStatus::MissionFailure);
			requestStackPush(GEX::StateID::GameOver);
		}
		_world.update(dt);
		GEX::CommandQueue& commands = _world.getCommandQueue();
		player.handleRealTimeInput(commands);
		return true;
	}
	bool Gamestate::handleEvents(const sf::Event & event)
	{
		GEX::CommandQueue& commands = _world.getCommandQueue();
		player.handleEvent(event,commands);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape)
			requestStackPush(GEX::StateID::Pause);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::G)
			requestStackPush(GEX::StateID::Gex);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Delete) { //set to delete rather than Q since Q is my rotate key
			requestStackClear();
			requestStackPush(GEX::StateID::Menu);
		}
		return true;
	}
