/*
File name : Splash.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Splash.h"			
#include "Screens.h"		
#include "..\Engine\Engine.h"
#include "..\Engine\Transform.h"

void Splash::Load() {

	Logo = Engine::GetTextureManager().Load("assets/DigiPen_BLACK_1024px.png");
}

void Splash::Update(double dt) {
	timer += dt;
	if (timer >= Time) {
		Engine::GetGameStateManager().SetNextState(MAINMENU);
	}

}

void Splash::UnLoad() {
	Logo = nullptr;
}

void Splash::Draw() {
	Engine::GetWindow().Clear(0xFFFFFFFF);
	Logo->Draw(TranslateMatrix(Vector2D<>{ (Engine::GetWindow().GetSize().x - static_cast<__int64>(Logo->GetSize().x)) / 2.0, (Engine::GetWindow().GetSize().y - static_cast<__int64>(Logo->GetSize().y)) / 2.0 }));
	
}
