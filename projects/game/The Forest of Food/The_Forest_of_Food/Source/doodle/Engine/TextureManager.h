/*
File name : TextureManager.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <string>
#include <map>

class Texture;

class TextureManager {
public:
	Texture* Load(const std::string& filePath);
	void Unload();

private:
	std::map<std::string, Texture*> pathToTexture;
};