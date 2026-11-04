/*
File name : Story.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "../Engine/GameState.h"
#include "..\Engine\Input.h"
#include "MainCharacter.h"
#include "Fairy.h"
#include "Inventory.h"
#include "Item.h"
#include "..\Engine/MouseClick.h"

class Texture;

class Story : public GameState
{
public:
	void Load() override;
	void Draw() override;
	void Update(double dt) override;
	void UnLoad() override;
	void StateChange();
	void conversation();
	void UpdateChooseMap();
	void SetChooseBox(int season);
	void MakePlayerName();
	void DrawChooseMap();
	

	Input::InputKey A_key{ Input::KeyboardButton::A };
	Input::InputKey B_key{ Input::KeyboardButton::B };
	Input::InputKey C_key{ Input::KeyboardButton::C };
	Input::InputKey D_key{ Input::KeyboardButton::D };
	Input::InputKey E_key{ Input::KeyboardButton::E };
	Input::InputKey F_key{ Input::KeyboardButton::F };
	Input::InputKey G_key{ Input::KeyboardButton::G };
	Input::InputKey H_key{ Input::KeyboardButton::H };
	Input::InputKey I_key{ Input::KeyboardButton::I };
	Input::InputKey J_key{ Input::KeyboardButton::J };
	Input::InputKey K_key{ Input::KeyboardButton::K };
	Input::InputKey L_key{ Input::KeyboardButton::L };
	Input::InputKey M_key{ Input::KeyboardButton::M };
	Input::InputKey N_key{ Input::KeyboardButton::N };
	Input::InputKey O_key{ Input::KeyboardButton::O };
	Input::InputKey P_key{ Input::KeyboardButton::P };
	Input::InputKey Q_key{ Input::KeyboardButton::Q };
	Input::InputKey R_key{ Input::KeyboardButton::R };
	Input::InputKey S_key{ Input::KeyboardButton::S };
	Input::InputKey T_key{ Input::KeyboardButton::T };
	Input::InputKey U_key{ Input::KeyboardButton::U };
	Input::InputKey V_key{ Input::KeyboardButton::V };
	Input::InputKey W_key{ Input::KeyboardButton::W };
	Input::InputKey X_key{ Input::KeyboardButton::X };
	Input::InputKey Y_key{ Input::KeyboardButton::Y };
	Input::InputKey Z_key{ Input::KeyboardButton::Z };
	Input::InputKey Back_Key{ Input::KeyboardButton::Backspace };
	Input::InputKey Caps_Key{ Input::KeyboardButton::CapsLock };

private:
	MainCharacter character;
	Fairy faity;
	int ConverationNum = 0;
	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
	Inventory* storyInven = nullptr;
	MouseClick mouseClick;
	Texture* ramen = nullptr;
	Texture* room = nullptr;

	Texture* spring = nullptr;
	Texture* summer = nullptr;
	Texture* fall = nullptr;
	Texture* winter = nullptr;

	
	bool boolRamenCheck = false;
	Item* itemRamen = nullptr;
	Rectangle rectRamenCheck = {{{0, 0}, {0, 0}}, {0, 0}};
	Rectangle rectSpringCheck = { {{0, 0}, {0, 0}}, {0, 0} };
	Rectangle rectSummerCheck = { {{0, 0}, {0, 0}}, {0, 0} };
	Rectangle rectFallCheck = { {{0, 0}, {0, 0}}, {0, 0} };
	Rectangle rectWinterCheck = { {{0, 0}, {0, 0}}, {0, 0} };

	bool OnCapsLock = false;
	ScaleMatrix scale = Vector2D<>{ 0.9, 0.9 };
	TransformMatrix objectMatrix;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	Rectangle NextCheck = { {{Engine::GetWindow().GetSize().x / 15.0, Engine::GetWindow().GetSize().y / 15.0}
		,{Engine::GetWindow().GetSize().x / 15.0 + (Engine::GetWindow().GetSize().x / 15.0) * 13.0
			, Engine::GetWindow().GetSize().y / 15.0 + (Engine::GetWindow().GetSize().y / 15.0) * 4.0}}, {0, 0} };

	Map map;
};

