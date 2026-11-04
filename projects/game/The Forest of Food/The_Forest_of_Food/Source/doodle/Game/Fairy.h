/*
File name : Fairy.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <doodle/doodle.hpp>
#include "../Engine/Engine.h"
#include "..\Engine\Transform.h"
#include "..\Engine/Texture.h"

enum class FairyFace {
	HUNGRY, HAPPY, DISAPPOINT, SOSO
};


class Fairy {
public:
	Fairy();
	void Draw();
	void SetFace(FairyFace some);
	void DrawFairyName();

private:
	double WindowX = Engine::GetWindow().GetSize().x;
	double WindowY = Engine::GetWindow().GetSize().y;
	const int pixelFontStyle = doodle::create_distance_field_bitmap_font("Font/DungGeunMo.fnt");
	
	ScaleMatrix scale;
	Vector2D<int> position;
	Texture* hungry;
	Texture* happy;
	Texture* disappoint;
	Texture* soso;

	TransformMatrix objectMatrix;
	FairyFace face;
};