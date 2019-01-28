/*
*@author: Greg VanKampen
*@file: ResourceIdentifiers.h
*@description: Holds IDs of resources
*/
#pragma once
namespace GEX {
	enum class TextureID { 
		Landscape,
		Jungle,
		Entities,
		LilyPad,
		Frog,
		TitleScreen, 
		GexScreen,
		Explosion,Particle,
		FinishLine,
		Buttons
	};
	enum class FontID {
		Main
	};
	enum class MusicID {
		MenuTheme,
		MissionTheme
	};
	enum class SoundEffectID {
		FroggerMove,
		FroggerDie,
		Jump,
	};
}