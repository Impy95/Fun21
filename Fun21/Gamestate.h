/**
*@author: Greg VanKampen
*@file: GameState.cpp
*@description: State when the game plays in
*/
#pragma once
#include "State.h"
#include "World.h"
#include "PlayerControl.h"

class Gamestate : public GEX::State
{
public:
	Gamestate(GEX::StateStack& stack, Context context);
	void draw() override;
	bool update(sf::Time dt) override;
	bool handleEvents(const sf::Event& event) override;
private:
	GEX::World _world;
	GEX::PlayerControl& player;

};

