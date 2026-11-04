/*
File name : Timer.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/Texture.h"
#include "doodle/doodle.hpp"

class Timer {
public:
	Timer();

	void Load();
	void Draw();
	void Update(double dt);
	void UnLoad();
	double GetTimeMin();
	double GetTimeSec();

private:
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	int minute;
	double second;
	double WindowX;
	double WindowY;
	double TimeRect;
	double OneSecond;

	Texture timer;
};