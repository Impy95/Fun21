/*
*@author: Greg VanKampen
*@file: Animation2
*@description: Advanced Animation Class
*/
#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <vector>
namespace sf {
	class Time;
}
using Frame = sf::IntRect;
namespace GEX {
	class Animation2 
	{
	public:
		explicit Animation2(bool repeat = true);
		Animation2(const sf::Texture& texture);

		void			addFrame(Frame frame);
		void			addFrameSet(std::vector<Frame> frames);
		void			setDuration(sf::Time duration);
		sf::Time		getDuration() const;
		void			setRepeating(bool flag);
		bool			isRepeating()const;
		Frame			getCurrentFrame() const;



		void			restart();
		bool			isFinished() const;

		Frame			update(sf::Time dt);

	private:
		std::vector<Frame> _frames;
		std::size_t		_currentFrame;
		sf::Time		_duration;
		sf::Time		_elapsedTime;
		bool			_repeat;
	};
}

