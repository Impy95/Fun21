/*
*@author: Greg VanKampen
*@file: State.h
*@description: A base state class
*/
#pragma once
#include "TextureManager.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "StateIdentifiers.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include <memory>
namespace GEX {
	class StateStack;
	class PlayerControl;
	class State {

		
	public:
		typedef std::unique_ptr<State> Ptr;
		struct Context {
			Context(sf::RenderWindow& window, TextureManager& textures, sf::Font& font, PlayerControl& player, MusicPlayer& music, SoundPlayer& sound);
			sf::RenderWindow*	window;
			TextureManager*		textures;
			PlayerControl*		player;
			MusicPlayer*		music;
			SoundPlayer*		sound;
		};
		State(StateStack& stack, Context context);
		virtual					~State();
		virtual void			draw() = 0;
		virtual bool			update(sf::Time) = 0;
		virtual bool			handleEvents(const sf::Event& event) = 0;
		Context					_context;
	protected:
		void					requestStackPush(StateID stateID);
		void					requestStackPop();
		void					requestStackClear();
		Context					getContext() const;

	private:
		StateStack *			_stack;
		
	};
}