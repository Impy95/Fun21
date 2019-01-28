#pragma once
#include <SFML\Audio\Music.hpp>
#include "ResourceIdentifiers.h"
#include <map>
#include <string>
namespace GEX {
	class MusicPlayer
	{
	public:
		MusicPlayer();
		~MusicPlayer() = default;
		MusicPlayer(const MusicPlayer&) = delete;
		MusicPlayer& operator=(const MusicPlayer&) = delete;

		void							play(MusicID theme);
		void							stop();
		void							setPaused(bool paused);
		void							setVolume(float volume);

	private:
		sf::Music						_music;
		std::map<MusicID, std::string>	_filenames;
		float							_volume;
	};
}

