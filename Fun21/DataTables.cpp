/**
*@author: Greg VanKampen & Vaugn Rowse
*@file: Datatable.cpp
*@description: Storage of attributes for entities
*/
#include "DataTables.h"
#include "JsonFrameParser.h"
#include <SFML\System\Time.hpp>
#include <SFML/Graphics/Transformable.hpp>



std::map<GEX::Pickup::Type, GEX::PickupData> GEX::initalizePickupData()
{
	std::map<GEX::Pickup::Type, GEX::PickupData> data;

	return data;
}

std::map<GEX::ObstacleType, GEX::ObstacleData> GEX::initalizeObstacleData()
{
	std::map<GEX::ObstacleType, GEX::ObstacleData> data;
	JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

	data[ObstacleType::Car1].texture = TextureID::Entities;
	data[ObstacleType::Car1].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("raceCarL"));
	data[ObstacleType::Car1].animations[Obstacle::State::Idle].setDuration(sf::seconds(1.f));
	data[ObstacleType::Car1].animations[Obstacle::State::Idle].setRepeating(true);
	//data[ObstacleType::Car1].textureRect = sf::IntRect(85, 337, 40, 40);
	data[ObstacleType::Car1].spawnTime = sf::seconds(4.5);

	data[ObstacleType::Car2].texture = TextureID::Entities;
	data[ObstacleType::Car2].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("raceCarL"));
	data[ObstacleType::Car2].animations[Obstacle::State::Idle].setDuration(sf::seconds(1.f));
	data[ObstacleType::Car2].animations[Obstacle::State::Idle].setRepeating(true);
	//data[ObstacleType::Car2].textureRect = sf::IntRect(43, 337, 40, 40);
	data[ObstacleType::Car2].spawnTime = sf::seconds(5.f);

	data[ObstacleType::Car3].texture = TextureID::Entities;
	data[ObstacleType::Car3].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("raceCarR"));
	data[ObstacleType::Car3].animations[Obstacle::State::Idle].setDuration(sf::seconds(1.f));
	data[ObstacleType::Car3].animations[Obstacle::State::Idle].setRepeating(true);
	//data[ObstacleType::Car3].textureRect = sf::IntRect(85, 337, 40, 40);
	data[ObstacleType::Car3].spawnTime = sf::seconds(3.f);

	data[ObstacleType::Truck].texture = TextureID::Entities;
	data[ObstacleType::Truck].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("truck"));
	data[ObstacleType::Truck].animations[Obstacle::State::Idle].setDuration(sf::seconds(1.f));
	data[ObstacleType::Truck].animations[Obstacle::State::Idle].setRepeating(true);
	//data[ObstacleType::Truck].textureRect = sf::IntRect(137, 237, 61, 40);
	data[ObstacleType::Truck].spawnTime = sf::seconds(5.f);

	data[ObstacleType::Tractor].texture = TextureID::Entities;
	data[ObstacleType::Tractor].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("tractor"));
	data[ObstacleType::Tractor].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::Tractor].animations[Obstacle::State::Idle].setRepeating(false);
	//data[ObstacleType::Tractor].textureRect = sf::IntRect(1, 379, 40, 40);
	data[ObstacleType::Tractor].spawnTime = sf::seconds(4.f);

	data[ObstacleType::Log1].texture = TextureID::Entities;
	data[ObstacleType::Log1].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("tree1"));
	data[ObstacleType::Log1].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::Log1].animations[Obstacle::State::Idle].setRepeating(false);
	data[ObstacleType::Log1].spawnTime = sf::seconds(2.5);

	data[ObstacleType::Log2].texture = TextureID::Entities;
	data[ObstacleType::Log2].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("tree2"));
	data[ObstacleType::Log2].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::Log2].animations[Obstacle::State::Idle].setRepeating(false);
	data[ObstacleType::Log2].spawnTime = sf::seconds(2.5);

	data[ObstacleType::GatorIcon].texture = TextureID::Entities;
	data[ObstacleType::GatorIcon].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("crocIcon"));
	data[ObstacleType::GatorIcon].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::GatorIcon].animations[Obstacle::State::Idle].setRepeating(false);


	data[ObstacleType::Log2_2].texture = TextureID::Entities;
	data[ObstacleType::Log2_2].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("tree1"));
	data[ObstacleType::Log2_2].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::Log2_2].animations[Obstacle::State::Idle].setRepeating(false);
	data[ObstacleType::Log2_2].spawnTime = sf::seconds(2.5);

	data[ObstacleType::Turtle2].texture = TextureID::Entities;
	data[ObstacleType::Turtle2].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("2turtles 1"));
	data[ObstacleType::Turtle2].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::Turtle2].animations[Obstacle::State::Idle].setRepeating(false);
	data[ObstacleType::Turtle2].spawnTime = sf::seconds(2.5);


	data[ObstacleType::Turtle3].texture = TextureID::Entities;
	data[ObstacleType::Turtle3].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("3turtles 1"));
	data[ObstacleType::Turtle3].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::Turtle3].animations[Obstacle::State::Idle].setRepeating(false);
	data[ObstacleType::Turtle3].spawnTime = sf::seconds(3);

	data[ObstacleType::Turtle3Diving].texture = TextureID::LilyPad;
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("3turtles"));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Idle].setDuration(sf::seconds(2.f));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Idle].setRepeating(true);

	data[ObstacleType::Gator].texture = TextureID::Entities;
	data[ObstacleType::Gator].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("croc "));
	data[ObstacleType::Gator].animations[Obstacle::State::Idle].setDuration(sf::seconds(1.f));
	data[ObstacleType::Gator].animations[Obstacle::State::Idle].setRepeating(true);

	data[ObstacleType::BonusFly].texture = TextureID::Entities;
	data[ObstacleType::BonusFly].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("bugIcon"));
	data[ObstacleType::BonusFly].animations[Obstacle::State::Idle].setDuration(sf::seconds(0.f));
	data[ObstacleType::BonusFly].animations[Obstacle::State::Idle].setRepeating(false);

	frames = JsonFrameParser("Media/Textures/LilyPad.json");
	data[ObstacleType::LilyPad].texture = TextureID::LilyPad;
	data[ObstacleType::LilyPad].animations[Obstacle::State::Idle].addFrameSet(frames.getFramesFor("lillyPad"));
	data[ObstacleType::LilyPad].animations[Obstacle::State::Idle].setDuration(sf::seconds(1.f));
	data[ObstacleType::LilyPad].animations[Obstacle::State::Idle].setRepeating(false);
	data[ObstacleType::FrogIcon].texture = TextureID::Entities;
	data[ObstacleType::FrogIcon].textureRect = sf::IntRect(127, 321,40,40);

	data[ObstacleType::Turtle2Diving].texture = TextureID::LilyPad;

	//turtles 2 aniamtions
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::Dive].addFrameSet(frames.getFramesFor("2turtles"));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::Dive].setDuration(sf::seconds(0.5));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::Dive].setRepeating(false);

	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::Surface].addFrameSet(frames.getFramesFor("2rturtles"));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::Surface].setDuration(sf::seconds(0.5));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::Surface].setRepeating(false);

	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::SwimUnder].addFrameSet(frames.getFramesFor("2turtles 4"));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::SwimUnder].setDuration(sf::seconds(0.5));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::SwimUnder].setRepeating(false);

	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::SwimOver].addFrameSet(frames.getFramesFor("2turtles 1"));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::SwimOver].setDuration(sf::seconds(2.f));
	data[ObstacleType::Turtle2Diving].animations[Obstacle::State::SwimOver].setRepeating(false);

	//turtle 3 animations
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Dive].addFrameSet(frames.getFramesFor("3turtles"));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Dive].setDuration(sf::seconds(1.f));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Dive].setRepeating(false);

	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Surface].addFrameSet(frames.getFramesFor("3rturtles"));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Surface].setDuration(sf::seconds(1.f));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::Surface].setRepeating(false);

	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::SwimUnder].addFrameSet(frames.getFramesFor("3turtles 4"));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::SwimUnder].setDuration(sf::seconds(1.f));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::SwimUnder].setRepeating(false);

	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::SwimOver].addFrameSet(frames.getFramesFor("3turtles 1"));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::SwimOver].setDuration(sf::seconds(3.f));
	data[ObstacleType::Turtle3Diving].animations[Obstacle::State::SwimOver].setRepeating(false);
	

	return data;
}


std::map<GEX::Particle::Type, GEX::ParticleData> GEX::initalizeParticleData()
{
	std::map<GEX::Particle::Type, GEX::ParticleData> data;
	data[GEX::Particle::Type::Propellant].color = sf::Color(50, 150, 100);
	data[GEX::Particle::Type::Propellant].lifetime = sf::seconds(0.6f);

	data[GEX::Particle::Type::Smoke].color = sf::Color(100, 150, 50);
	data[GEX::Particle::Type::Smoke].lifetime = sf::seconds(0.8f);

	return data;
}

GEX::FrogData GEX::intitalizeFrogData()
{
	GEX::FrogData frogData;
	JsonFrameParser frames = JsonFrameParser("Media/Textures/Atlas.json");

	frogData.texture = TextureID::Frog;
	frogData.textureRect = sf::IntRect(43, 379, 40, 40);
	frogData.animations[GEX::Frog::State::Up].addFrameSet(frames.getFramesFor("up"));
	frogData.animations[GEX::Frog::State::Up].setDuration(sf::Time(sf::seconds(1.f)));
	frogData.animations[GEX::Frog::State::Up].setRepeating(false);
	frogData.animations[GEX::Frog::State::Down].addFrameSet(frames.getFramesFor("down"));
	frogData.animations[GEX::Frog::State::Down].setDuration(sf::Time(sf::seconds(1.f)));
	frogData.animations[GEX::Frog::State::Down].setRepeating(false);
	frogData.animations[GEX::Frog::State::Left].addFrameSet(frames.getFramesFor("left"));
	frogData.animations[GEX::Frog::State::Left].setDuration(sf::Time(sf::seconds(1.f)));
	frogData.animations[GEX::Frog::State::Left].setRepeating(false);
	frogData.animations[GEX::Frog::State::Right].addFrameSet(frames.getFramesFor("right"));
	frogData.animations[GEX::Frog::State::Right].setDuration(sf::Time(sf::seconds(1.f)));
	frogData.animations[GEX::Frog::State::Right].setRepeating(false);
	frogData.animations[GEX::Frog::State::Die].addFrameSet(frames.getFramesFor("die"));
	frogData.animations[GEX::Frog::State::Die].setDuration(sf::Time(sf::seconds(2.f)));
	frogData.animations[GEX::Frog::State::Die].setRepeating(false);

	return frogData;
}

std::map<GEX::CardType, GEX::CardData> GEX::initializeCardData()
{
	std::map<GEX::CardType, GEX::CardData> data;

	//const auto cardTypeSize = int(CardType::COUNT_AT_END);
	//std::vector<CardType> cardTypes;
	//for (int i = 0; i < cardTypeSize; i++)
	//{
	//	cardTypes.push_back(CardType(i));
	//}

	//for (int i = 0; i < cardTypes.size(); i++)
	//{
	//	data[cardTypes[i]].texture = TextureID::Cards;
	//	data[cardTypes[i]].textureRect = sf::IntRect(0, 0, 301, 469);
	//}

	// Aces
	data[CardType::AceClub].texture = TextureID::Cards;
	data[CardType::AceClub].textureRect = sf::IntRect(0, 0, 301, 469);

	data[CardType::AceDiamond].texture = TextureID::Cards;
	data[CardType::AceDiamond].textureRect = sf::IntRect(0, 469, 301, 469);

	data[CardType::AceHeart].texture = TextureID::Cards;
	data[CardType::AceHeart].textureRect = sf::IntRect(0, 938, 301, 469);

	data[CardType::AceSpade].texture = TextureID::Cards;
	data[CardType::AceSpade].textureRect = sf::IntRect(0, 1407, 301, 469);

	// Twos
	data[CardType::TwoClub].texture = TextureID::Cards;
	data[CardType::TwoClub].textureRect = sf::IntRect(301, 0, 301, 469);

	data[CardType::TwoDiamond].texture = TextureID::Cards;
	data[CardType::TwoDiamond].textureRect = sf::IntRect(301, 469, 301, 469);

	data[CardType::TwoHeart].texture = TextureID::Cards;
	data[CardType::TwoHeart].textureRect = sf::IntRect(301, 938, 301, 469);

	data[CardType::TwoSpade].texture = TextureID::Cards;
	data[CardType::TwoSpade].textureRect = sf::IntRect(301, 1407, 301, 469);

	// Threes
	data[CardType::ThreeClub].texture = TextureID::Cards;
	data[CardType::ThreeClub].textureRect = sf::IntRect(602, 0, 301, 469);

	data[CardType::ThreeDiamond].texture = TextureID::Cards;
	data[CardType::ThreeDiamond].textureRect = sf::IntRect(602, 469, 301, 469);

	data[CardType::ThreeHeart].texture = TextureID::Cards;
	data[CardType::ThreeHeart].textureRect = sf::IntRect(602, 938, 301, 469);

	data[CardType::ThreeSpade].texture = TextureID::Cards;
	data[CardType::ThreeSpade].textureRect = sf::IntRect(602, 1407, 301, 469);

	// Fours
	data[CardType::FourClub].texture = TextureID::Cards;
	data[CardType::FourClub].textureRect = sf::IntRect(903, 0, 301, 469);

	data[CardType::FourDiamond].texture = TextureID::Cards;
	data[CardType::FourDiamond].textureRect = sf::IntRect(903, 469, 301, 469);

	data[CardType::FourHeart].texture = TextureID::Cards;
	data[CardType::FourHeart].textureRect = sf::IntRect(903, 938, 301, 469);

	data[CardType::FourSpade].texture = TextureID::Cards;
	data[CardType::FourSpade].textureRect = sf::IntRect(903, 1407, 301, 469);

	// Fives
	data[CardType::FiveClub].texture = TextureID::Cards;
	data[CardType::FiveClub].textureRect = sf::IntRect(1204, 0, 301, 469);

	data[CardType::FiveDiamond].texture = TextureID::Cards;
	data[CardType::FiveDiamond].textureRect = sf::IntRect(1204, 469, 301, 469);

	data[CardType::FiveHeart].texture = TextureID::Cards;
	data[CardType::FiveHeart].textureRect = sf::IntRect(1204, 938, 301, 469);

	data[CardType::FiveSpade].texture = TextureID::Cards;
	data[CardType::FiveSpade].textureRect = sf::IntRect(1204, 1407, 301, 469);

	// Sixes
	data[CardType::SixClub].texture = TextureID::Cards;
	data[CardType::SixClub].textureRect = sf::IntRect(1505, 0, 301, 469);

	data[CardType::SixDiamond].texture = TextureID::Cards;
	data[CardType::SixDiamond].textureRect = sf::IntRect(1505, 469, 301, 469);

	data[CardType::SixHeart].texture = TextureID::Cards;
	data[CardType::SixHeart].textureRect = sf::IntRect(1505, 938, 301, 469);

	data[CardType::SixSpade].texture = TextureID::Cards;
	data[CardType::SixSpade].textureRect = sf::IntRect(1505, 1407, 301, 469);

	// Sevens
	data[CardType::SevenClub].texture = TextureID::Cards;
	data[CardType::SevenClub].textureRect = sf::IntRect(1806, 0, 301, 469);

	data[CardType::SevenDiamond].texture = TextureID::Cards;
	data[CardType::SevenDiamond].textureRect = sf::IntRect(1806, 469, 301, 469);

	data[CardType::SevenHeart].texture = TextureID::Cards;
	data[CardType::SevenHeart].textureRect = sf::IntRect(1806, 938, 301, 469);

	data[CardType::SevenSpade].texture = TextureID::Cards;
	data[CardType::SevenSpade].textureRect = sf::IntRect(1806, 1407, 301, 469);

	// Eights
	data[CardType::EightClub].texture = TextureID::Cards;
	data[CardType::EightClub].textureRect = sf::IntRect(2107, 0, 301, 469);

	data[CardType::EightDiamond].texture = TextureID::Cards;
	data[CardType::EightDiamond].textureRect = sf::IntRect(2107, 469, 301, 469);

	data[CardType::EightHeart].texture = TextureID::Cards;
	data[CardType::EightHeart].textureRect = sf::IntRect(2107, 938, 301, 469);

	data[CardType::EightSpade].texture = TextureID::Cards;
	data[CardType::EightSpade].textureRect = sf::IntRect(2107, 1407, 301, 469);

	// Nines
	data[CardType::NineClub].texture = TextureID::Cards;
	data[CardType::NineClub].textureRect = sf::IntRect(2408, 0, 301, 469);

	data[CardType::NineDiamond].texture = TextureID::Cards;
	data[CardType::NineDiamond].textureRect = sf::IntRect(2408, 469, 301, 469);

	data[CardType::NineHeart].texture = TextureID::Cards;
	data[CardType::NineHeart].textureRect = sf::IntRect(2408, 938, 301, 469);

	data[CardType::NineSpade].texture = TextureID::Cards;
	data[CardType::NineSpade].textureRect = sf::IntRect(2408, 1407, 301, 469);

	// Tens
	data[CardType::TenClub].texture = TextureID::Cards;
	data[CardType::TenClub].textureRect = sf::IntRect(2709, 0, 301, 469);

	data[CardType::TenDiamond].texture = TextureID::Cards;
	data[CardType::TenDiamond].textureRect = sf::IntRect(2709, 469, 301, 469);

	data[CardType::TenHeart].texture = TextureID::Cards;
	data[CardType::TenHeart].textureRect = sf::IntRect(2709, 938, 301, 469);

	data[CardType::TenSpade].texture = TextureID::Cards;
	data[CardType::TenSpade].textureRect = sf::IntRect(2709, 1407, 301, 469);

	// Jacks
	data[CardType::JackClub].texture = TextureID::Cards;
	data[CardType::JackClub].textureRect = sf::IntRect(3010, 0, 301, 469);

	data[CardType::JackDiamond].texture = TextureID::Cards;
	data[CardType::JackDiamond].textureRect = sf::IntRect(3010, 469, 301, 469);

	data[CardType::JackHeart].texture = TextureID::Cards;
	data[CardType::JackHeart].textureRect = sf::IntRect(3010, 938, 301, 469);

	data[CardType::JackSpade].texture = TextureID::Cards;
	data[CardType::JackSpade].textureRect = sf::IntRect(3010, 1407, 301, 469);

	// Queens
	data[CardType::QueenClub].texture = TextureID::Cards;
	data[CardType::QueenClub].textureRect = sf::IntRect(3311, 0, 301, 469);

	data[CardType::QueenDiamond].texture = TextureID::Cards;
	data[CardType::QueenDiamond].textureRect = sf::IntRect(3311, 469, 301, 469);

	data[CardType::QueenHeart].texture = TextureID::Cards;
	data[CardType::QueenHeart].textureRect = sf::IntRect(3311, 938, 301, 469);

	data[CardType::QueenSpade].texture = TextureID::Cards;
	data[CardType::QueenSpade].textureRect = sf::IntRect(3311, 1407, 301, 469);

	return data;
}



