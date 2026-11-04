/*
File name : SoundManager.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <SFML/Audio.hpp>
#include <map>


class SoundManager
{
	class SFXManager
	{
		std::map<std::string, sf::SoundBuffer*> pathToBuffer;
	public:
		sf::SoundBuffer* Load(const std::string& filePath);
		void Unload();
	};

	class BGMManager
	{
		std::map<std::string, sf::Music*> pathToMusic;

	public:
		sf::Music* Load(const std::string& filePath);
		void Unload();
	};

	SFXManager sfxManager;
	BGMManager bgmManager;

public:
	sf::SoundBuffer* LoadSFX(const std::string& filePath);
	sf::Music* LoadBGM(const std::string& filePath);

	void Unload();

};