/*
File name : Credit.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Credit.h"
#include "../Engine/Engine.h"
#include "Screens.h"

using namespace doodle;

CreditScreen::CreditScreen() :Background(nullptr) {}

void CreditScreen::Load()
{
	Background = Engine::GetTextureManager().Load("assets/Credittexture.png");
}

void CreditScreen::Update(double)
{
	Engine::GetWindow().Clear(0x6ab6f6ff);
	if (mouseClick.MouseOneClick())
	{
		Engine::GetGameStateManager().SetNextState(MAINMENU);
	}

}

void CreditScreen::Draw()
{
	Background->Draw(TranslateMatrix(Vector2D<>{ (Engine::GetWindow().GetSize().x - static_cast<__int64>(Background->GetSize().x)) / 2.0, (Engine::GetWindow().GetSize().y - static_cast<__int64>(Background->GetSize().y)) / 2.0 }));
}

void CreditScreen::UnLoad()
{
	Background = nullptr;
}