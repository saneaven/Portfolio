/*
File name : Fall.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
Author(Second) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Fairy.h"
#include "MainCharacter.h"
#include "../Engine/GameState.h"
#include "../Engine/GameObjectManager.h"
#include "../Engine/MouseClick.h"
#include "Interface.h"


class Fall :public GameState
{
public:
	Fall();

	void Load() override;
	void Draw() override;
	void Update(double dt) override;
	void UnLoad() override;
	void conversation();

	/// <summary>
	Input::InputKey SetSpring{ Input::KeyboardButton::U };
	Input::InputKey SetSummer{ Input::KeyboardButton::I };
	Input::InputKey SetFall{ Input::KeyboardButton::O };
	Input::InputKey SetWinter{ Input::KeyboardButton::P };
	/// </summary>

private:
	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
	int ConverationNum = 0;
	TransformMatrix objectMatrix;
	Rectangle NextCheck = { {{Engine::GetWindow().GetSize().x / 15.0, Engine::GetWindow().GetSize().y / 15.0}
		,{Engine::GetWindow().GetSize().x / 15.0 + (Engine::GetWindow().GetSize().x / 15.0) * 13.0
			, Engine::GetWindow().GetSize().y / 15.0 + (Engine::GetWindow().GetSize().y / 15.0) * 4.0}}, {0, 0} };
	bool gameStart = false;
	MainCharacter mainCharacter;
	Fairy faity;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	
	GameObjectManager objectManager;
	MouseClick mouseClick;
	Texture* Hint;
	Interface interface;
};

