/*
*@author: Greg VanKampen
*@file: SoundPlayer.h
*@description: A class which plays sound
*/
#pragma once
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\SoundBuffer.hpp>
#include <SFML\System\Vector2.hpp>
#include"ResourceIdentifiers.h"
#include <SFML\Audio\Listener.hpp>
#include <memory>
#include <map>
#include <list>
namespace GEX {
	class SoundPlayer
	{
	public:
		SoundPlayer();
		~SoundPlayer() = default;
		SoundPlayer(const SoundPlayer&) = delete;
		SoundPlayer& operator=(const SoundPlayer&) = delete;

		void				play(SoundEffectID effect);
		void				play(SoundEffectID effect, sf::Vector2f position);

		void				removeStoppedSounds();
		void				setListenerPosition(sf::Vector2f position);
		sf::Vector2f		getListenerPosition() const;
	private:
		void				loadBuffer(SoundEffectID id, const std::string path);


	private:
		std::map<SoundEffectID, std::unique_ptr<sf::SoundBuffer>>	_soundsBuffers;
		std::list<sf::Sound>										_sounds;
	};
}

