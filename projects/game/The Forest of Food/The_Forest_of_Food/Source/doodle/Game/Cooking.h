/*
File name : Cooking.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include "../Engine/GameState.h"
#include "../Engine/GameObjectManager.h"
#include "..\Engine\Rectangle.h"
#include "MainCharacter.h"
#include "../Engine/MouseClick.h"


 inline int CheckSeaonCooking;

struct Square : Rectangle {
	using Rectangle::Rectangle;
	bool OnClick = false;
	bool have = false;
};

struct CookingItem : public Texture {
	CookingItem(const std::filesystem::path& filePath, bool have = false): Texture(filePath), have(have){};
	bool have;
};


class Cooking : public GameState {
public:
	Cooking();

	void Load() override;
	void Draw() override;
	void Update(double dt) override;
	void UnLoad() override;
	void conversation();
	void CheckItems();
	void MakeCookingBox();
	void DrawCookingBox();
	void on_mouse_released(doodle::MouseButtons mouse);
	void SetIems();
	void PushItems(const std::filesystem::path& filePath, bool some);
	void controlMaterialBool(bool some);
	bool CheckFood();
private:
	Input::InputKey openMaterial{ Input::KeyboardButton::Enter };
	MainCharacter maincharacter;
	Camera cameraMatrix;
	bool finishChoose = false;
	std::vector<Square> square;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	Rectangle NextCheck = { {{Engine::GetWindow().GetSize().x / 15.0, Engine::GetWindow().GetSize().y / 15.0}
	,{Engine::GetWindow().GetSize().x / 15.0 + (Engine::GetWindow().GetSize().x / 15.0) * 13.0
		, Engine::GetWindow().GetSize().y / 15.0 + (Engine::GetWindow().GetSize().y / 15.0) * 4.0}}, {0, 0} };
	MouseClick mouseClick;
	double EndCookingTime = 3.0;
	Sprite CookingPot;

	Texture* Inven = nullptr;
	Texture* Mark = nullptr;
	int ConverationNum = 0;
	double WindowX = 0;
	double WindowY = 0;
	double MouseX = 0;
	double MouseY = 0;
	
	int height = 4;
	int width = 7;
	int BoxSize = 128;

	bool MouseRelese = false;		
	bool breakMouseClick = true;

	bool rice = false;				//1
	bool chwinalmul = false;		//2
	bool cabbage = false;			//3
	bool carrot = false;			//4
	bool sedum = false;				//5
	bool egg = false;				//6
	bool purse = false;				//7
	bool pine = false;				//8
	bool enoki = false;				//9
	bool garlic = false;			//10
	bool ginseng = false;			//11
	bool glutinous = false;			//12
	bool green_onion = false;		//13
	bool potato = false;			//14
	bool tofu = false;				//15
	bool butter = false;			//16
	bool onion = false;				//17
	bool turkey = false;			//18
	bool chicken = false;			//19
	bool jujube = false;			//20
	bool beef = false;				//21
	bool pork = false;				//22
	bool brocooli = false;			//23
	bool cucumber = false;			//24
	bool eggplant = false;			//25
	bool pepper = false;			//26
	bool radish = false;			//27
	bool ramen = false;				//28


	std::vector<CookingItem*> materials;
};