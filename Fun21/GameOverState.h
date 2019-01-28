/*
*@author: Greg VanKampen
*@file: GameOverState.cpp
*@description: State when the game ends
*/
#pragma once
#include "State.h"
class GameOverState : public GEX::State
{
public:
	GameOverState(GEX::StateStack& stack, Context context);

	void					draw() override;
	bool					update(sf::Time dt) override;
	bool					handleEvents(const sf::Event& event) override;

private:
	sf::Text				_gameOverText;
	sf::Time				_elapsedtime;
private:
	
};

