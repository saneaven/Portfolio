/*
File name : Splash.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "..\Engine\GameState.h"	
#include "..\Engine\Texture.h"		


class Splash : public GameState {
public:
	void Load() override;
	void Update(double dt) override;
	void UnLoad() override;
	void Draw() override;

private:
	const double Time = 3.0;
	double timer = 0;
	Texture *Logo = nullptr;
};