/*
File name : Ending.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
Author(Second) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include <doodle/doodle.hpp>
#include "Fairy.h"
#include "MainCharacter.h"
#include "../Engine/GameState.h"
#include "../Engine/Input.h"
#include "../Engine/MouseClick.h"
class Texture;


class GoodEnding :public GameState
{
public:
	void Load() override;
	void Draw() override;
	void UnLoad() override;
	void Update(double dt) override;
	
	void conversation();
	void conversationWindow();


private:
	MainCharacter maincharacter;
	Fairy fairy;
	const double font_size = 56;
	Input::InputKey next{ Input::KeyboardButton::R };

	
	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
	int ConverationNum = 0;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	MouseClick mouse;
	Texture* ending = nullptr;

		
};

class BadEnding :public GameState
{
public:
	void Load() override;
	void Draw() override;
	void UnLoad() override;
	void Update(double dt) override;

	void conversation();
	void conversationWindow();

private:
	MainCharacter maincharacter;
	Fairy fairy;
	const double font_size = 56;
	Input::InputKey next{ Input::KeyboardButton::R };

	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
	int ConverationNum = 0;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	MouseClick mouse;
	Texture* ending = nullptr;
	
};

class HiddenEnding :public GameState {
public:
	void Load() override;
	void Draw() override;
	void UnLoad() override;
	void Update(double dt) override;

	void conversation();
	void conversationWindow();

private:
	Fairy fairy;
	const double font_size = 56;
	Input::InputKey next{ Input::KeyboardButton::R };

	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
	int ConverationNum = 0;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	MouseClick mouse;
	Texture* ending = nullptr;
	Texture* hiddenEnding = nullptr;
};