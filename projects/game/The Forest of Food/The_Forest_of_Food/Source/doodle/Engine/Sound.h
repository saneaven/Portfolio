/*
File name : Sound.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <SFML/Audio.hpp>


class SFX
{
	sf::Sound sound;
public:
	SFX(const std::string& filePath, bool soundRelative = false);
	void Play();
	void PlayOnly();
	bool isRelative();
};

class BGM
{
	sf::Music* music;

public:
	BGM(const std::string& filePath, bool loop = true);

	void Play();
	void Stop();
	bool isLoop();
};