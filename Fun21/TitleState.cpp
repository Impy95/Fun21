/*
*@author: Greg VanKampen
*@file: TitleState.cpp
*@description: State shown on game start up
*/
#include "TitleState.h"
#include "TextureManager.h"
#include "Utility.h"
#include "FontManager.h"

TitleState::TitleState(GEX::StateStack & stack, Context context)
	:State(stack,context),
	_text(),
	_showText(true),
	_textEffectTime(sf::Time::Zero)
{
	_backgroundSprite.setTexture(context.textures->get(GEX::TextureID::TitleScreen));
	_text.setFont(GEX::FontManager::getInstance().getFont(GEX::FontID::Main));
	_text.setString("PRESS ANY KEY TO START!");
	_text.setFillColor(sf::Color::Red);
	centerOrigin(_text);
	_text.setPosition(context.window->getView().getSize().x / 2.f, context.window->getView().getSize().y * 0.75);

	context.music->play(GEX::MusicID::MenuTheme);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(_backgroundSprite);

	if (_showText) {
		window.draw(_text);
	}
}

bool TitleState::update(sf::Time dt)
{
	_textEffectTime += dt;
	if (_textEffectTime >= sf::seconds(0.1)) {
		_showText = !_showText;
		_textEffectTime = sf::Time::Zero;
	}
	return true;
}

bool TitleState::handleEvents(const sf::Event & event)
{
	if (event.type == sf::Event::KeyPressed) {
		requestStackPop();
		requestStackPush(GEX::StateID::Game);
	}

	return true;

}
