/*
*@author: Greg VanKampen
*@file: Source.cpp
*@description: a base class which inits the application
*/
#include <SFML/Graphics.hpp>
#include "Application.h"
int main()
{
	Application game;
	game.run();
	return 0;
}