/*
File name : Main_UI.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Sieun Choi
Author(Second):MinJae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <doodle/doodle.hpp>
#include "../Engine/MouseClick.h"
#include "../Engine/Texture.h"
#include "..\Engine\GameState.h"

class MainMenu :public GameState{
public:
	MainMenu();
	void Load()override;
	void Draw()override ;
	void Update(double dt) override;
	void UnLoad() override;
	void Credit();

private:
	MouseClick mouseClick;
	Texture* Main;
	Texture* MainFullScreen;
	Vector2D<> window_s;
	int setstate;
	void Box(Vector2D<>,Vector2D<>,int state);
	const int font_style = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");

	double animationValue, animationValue2;

	Vector2D<> BoxSize = { 400,100 };
	Texture* ChoiceBox;
	Texture* Colored_Box;
	Texture* Point;
	bool Full;
};
