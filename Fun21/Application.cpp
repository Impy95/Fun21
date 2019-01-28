/*
*@author: Greg VanKampen
*@file: Application.cpp
*@description: Main game application controller
*/
#include "Application.h"
#include "State.h"
#include "FontManager.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.0f / 60.0f);

Application::Application()
	:_window(sf::VideoMode(1020,600),"Fun 21",sf::Style::Close),
	_player(),
	_font(),
	_textures(),
	_stateStack(GEX::State::Context(_window,_textures,_font,_player,_music,_sound)), 
	_statNumFrames(0)
{
	_window.setKeyRepeatEnabled(false);
	GEX::FontManager::getInstance().load(GEX::FontID::Main,"Media/apple_kid.ttf");
	_textures.load(GEX::TextureID::TitleScreen, "Media/Textures/FroggerTitle.png");
	_textures.load(GEX::TextureID::GexScreen, "Media/Textures/face.png");


	///StatDisplay

	/*_statText.setFont(GEX::FontManager::getInstance().getFont(GEX::FontID::Main));
	_statText.setPosition(5.0f, 5.0f);
	_statText.setCharacterSize(20);
	_statText.setString("FPS:\nUpdateTime:\nX:Y:R:");*/

	registerStates();
	_stateStack.pushState(GEX::StateID::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (_window.isOpen())
	{
		
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TimePerFrame)
		{
			processInput();
			if (_stateStack.isEmpty())
				_window.close();
			update(TimePerFrame);
			timeSinceLastUpdate -= TimePerFrame;
		}

		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (_window.pollEvent(event)) {
		_stateStack.handleEvent(event);
		if (event.type == sf::Event::Closed)
			_window.close();
	}
}

void Application::update(sf::Time deltaTime)
{
	_stateStack.update(deltaTime);

}

void Application::render()
{
	_window.clear();
	_stateStack.draw();
	_window.setView(_window.getDefaultView());
	_window.display();
}

void Application::registerStates()
{
	_stateStack.registerState<TitleState>(GEX::StateID::Title),	_stateStack.registerState<MenuState>(GEX::StateID::Menu);
	_stateStack.registerState<Gamestate>(GEX::StateID::Game);
	_stateStack.registerState<PauseState>(GEX::StateID::Pause);
	_stateStack.registerState<GexState>(GEX::StateID::Gex);
	_stateStack.registerState<GameOverState>(GEX::StateID::GameOver);
}
