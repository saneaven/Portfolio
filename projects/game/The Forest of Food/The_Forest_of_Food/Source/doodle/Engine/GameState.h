/*
File name : GameState.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi, Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
//interface abstract class

#include <string>

class BGM;

class GameState {
	BGM* bgm;

public:
	virtual void Load() = 0;
	virtual void Update(double dt) = 0;
	virtual void UnLoad() = 0;
	virtual void Draw() = 0;

	void SetBGM(std::string filePath);
	void UnloadBGM();
};