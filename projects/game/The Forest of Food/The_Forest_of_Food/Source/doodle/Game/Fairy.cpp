/*
File name : Fairy.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Fairy.h"
#include "../Engine/Engine.h"

Fairy::Fairy() : position({ 800, 500 }), scale({ 0.25, 0.25 }) {
	hungry = Engine::GetTextureManager().Load("Character/Fairy_Hungry.png");
	happy = Engine::GetTextureManager().Load("Character/Fairy_Happy.png");
	soso = Engine::GetTextureManager().Load("Character/fairy_soso.png");
	disappoint = Engine::GetTextureManager().Load("Character/fairy_disappoint.png");

	objectMatrix = TranslateMatrix(position) * scale;
	face = FairyFace::SOSO;
}

void Fairy::Draw() {
	switch (face) {
	case FairyFace::HAPPY:
		happy->Draw(objectMatrix);
		break;
	case FairyFace::HUNGRY:
		hungry->Draw(objectMatrix);
		break;
	case FairyFace::SOSO:
		soso->Draw(objectMatrix);
		break;
	case FairyFace::DISAPPOINT:
		disappoint->Draw(objectMatrix);
		break;
	}
}

void Fairy::SetFace(FairyFace some) {
	face = some;
}

void Fairy::DrawFairyName() {
	doodle::push_settings();
	doodle::set_font(pixelFontStyle);
	doodle::set_fill_color(doodle::HexColor{ 0xFF748Cff });
	doodle::set_font_size(48);
	doodle::draw_text("Fairy", (Engine::GetWindow().GetSize().x / 15.0) + 10, (Engine::GetWindow().GetSize().y / 3.0) + 5);
	doodle::pop_settings();
}
