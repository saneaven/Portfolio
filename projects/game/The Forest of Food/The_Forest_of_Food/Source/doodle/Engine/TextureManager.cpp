/*
File name : TextureManager.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Engine.h"
#include "Texture.h"
#include "TextureManager.h"


Texture* TextureManager::Load(const std::string& filePath) {
	if (pathToTexture.find(filePath) == pathToTexture.end()) {
		pathToTexture[filePath] = new Texture(filePath);
	}
	return pathToTexture[filePath];
}

void TextureManager::Unload() {

	for (std::pair<std::string, Texture*> texturePtr : pathToTexture) {
		delete texturePtr.second;
	}
	pathToTexture.clear();
}
