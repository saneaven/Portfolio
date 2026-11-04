/*
File name : Credit.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/GameState.h"
#include "../Engine/MouseClick.h"
#include "../Engine/Texture.h"

class CreditScreen :public GameState
{
public:
	CreditScreen();
	void Load() override;
	void Update(double dt) override;
	void Draw() override;
	void UnLoad() override;
private:
	MouseClick mouseClick;
	Texture* Background;
};
