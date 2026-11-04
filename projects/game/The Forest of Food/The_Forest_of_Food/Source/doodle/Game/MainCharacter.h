/*
File name : MainCharacter.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include <doodle/doodle.hpp>
#include "../Engine/Engine.h"
#include "../Engine/Transform.h"
#include "..\Engine/Texture.h"

enum class MainCharacterFace {
	HUNGRY, HAPPY, DISAPPOINT, SOSO
};

std::string inline PlayerName = "";

class MainCharacter {
public:
	MainCharacter();
	void Load();
	void Draw();
	void SetFace(MainCharacterFace some);
	void SetPosition(Vector2D<> pos);
	void SetScale(double x, double y);
	void DrawPlayerNameWithEngine();
	void DrawPlayerNameWithLoad();

private:
	double WindowX;
	double WindowY;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	ScaleMatrix scale;
	Vector2D<int> position;
	Texture* disappoint;
	Texture* soso;
	Texture* hungry;
	Texture* happy;

	TransformMatrix objectMatrix;
	MainCharacterFace face;
};