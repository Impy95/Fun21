/*
*@author: Vaughn Rowse
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
		HitButton,
		StayButton,
		DoubleButton,
		SplitButton,
		Cards,
		Bet5Button,
		Bet25Button,
		BetMaxButton,
		DealButton,
		ClearBetButton,
		CardBack,
		ArrowIndicator
	};
	enum class FontID {
		Main
	};
	enum class MusicID {
		MenuTheme,
		GameTheme
	};
	enum class SoundEffectID {
		FroggerMove,
		FroggerDie,
		Jump,
		ButtonClick,
		DealCard,
	};
}