/*
*@author: Greg VanKampen
*@file: Animation2
*@description: Advanced Animation Class
*/
#include "Animation2.h"
GEX::Animation2::Animation2(bool repeat)
	:_frames(),
	_currentFrame(0),
	_duration(sf::Time::Zero),
	_elapsedTime(sf::Time::Zero),
	_repeat(repeat)

{
}

GEX::Animation2::Animation2(const sf::Texture & texture)
{
	_duration = _duration;
}

void GEX::Animation2::addFrame(Frame frame)
{
	_frames.emplace_back(frame);
}

void GEX::Animation2::addFrameSet(std::vector<Frame> frames)
{
	_frames = frames;
}


void GEX::Animation2::setDuration(sf::Time duration)
{
	_duration = duration;
}

sf::Time GEX::Animation2::getDuration() const
{
	return _duration;
}

void GEX::Animation2::setRepeating(bool flag)
{
	_repeat = flag;
}

bool GEX::Animation2::isRepeating() const
{
	return _repeat;
}

Frame GEX::Animation2::getCurrentFrame() const
{
	return _frames[_currentFrame >= _frames.size() ? _frames.size() -1 : _currentFrame];
}


void GEX::Animation2::restart()
{
	_currentFrame = 0;
}

bool GEX::Animation2::isFinished() const
{
	return !_repeat && _currentFrame >= _frames.size();
}

Frame GEX::Animation2::update(sf::Time dt)
{
	sf::Time timePerFrame = _duration / static_cast<float>(_frames.size());
	_elapsedTime += dt;

	while (_elapsedTime >= timePerFrame && (_currentFrame <= _frames.size() || _repeat)) {
		_currentFrame++;
		_elapsedTime -= timePerFrame;
		if (_repeat) {
			_currentFrame = _currentFrame % _frames.size();

		}
	}
	return getCurrentFrame();
}


