#include "MusicPlayer.h"

GEX::MusicPlayer::MusicPlayer()
	:_music(),
	_filenames(),
	_volume(25)
{
	_filenames[MusicID::MenuTheme] = "Media/Music/MenuTheme.ogg";
	_filenames[MusicID::MissionTheme] = "Media/Music/MissionTheme.ogg";
}

void GEX::MusicPlayer::play(MusicID theme)
{
	if (!_music.openFromFile(_filenames[theme]))
		throw std::runtime_error("Music could not open file");
	_music.setVolume(_volume);
	_music.setLoop(true);
	_music.play();
}

void GEX::MusicPlayer::stop()
{
	_music.stop();
}

void GEX::MusicPlayer::setPaused(bool paused)
{
	if (paused)
		_music.pause();
	else
		_music.play();
}

void GEX::MusicPlayer::setVolume(float volume)
{
	_volume = volume;
	_music.setVolume(_volume);
}


