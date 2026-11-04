/*
File name : SoundManager.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "SoundManager.h"



sf::SoundBuffer* SoundManager::LoadSFX(const std::string& filePath)
{
	return sfxManager.Load(filePath);
}
sf::Music* SoundManager::LoadBGM(const std::string& filePath)
{
	return bgmManager.Load(filePath);
}

void SoundManager::Unload()
{
	sfxManager.Unload();
	bgmManager.Unload();
}

sf::SoundBuffer* SoundManager::SFXManager::Load(const std::string& filePath)
{
	if (pathToBuffer.find(filePath) == pathToBuffer.end())
	{
		pathToBuffer[filePath] = new sf::SoundBuffer;
		pathToBuffer[filePath]->loadFromFile(filePath);
	}

	return pathToBuffer[filePath];
}

void SoundManager::SFXManager::Unload()
{

	for (std::pair<std::string, sf::SoundBuffer*> bufferPtr : pathToBuffer)
	{
		delete bufferPtr.second;
	}
	pathToBuffer.clear();
}


sf::Music* SoundManager::BGMManager::Load(const std::string& filePath)
{
	if (pathToMusic.find(filePath) == pathToMusic.end())
	{
		pathToMusic[filePath] = new sf::Music;
		pathToMusic[filePath]->openFromFile(filePath);
	}

	return pathToMusic[filePath];
}

void SoundManager::BGMManager::Unload()
{

	for (std::pair<std::string, sf::Music*> musicPtr : pathToMusic)
	{
		delete musicPtr.second;
	}
	pathToMusic.clear();
}