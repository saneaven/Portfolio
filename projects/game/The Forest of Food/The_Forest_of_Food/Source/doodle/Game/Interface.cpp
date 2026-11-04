/*
File name : Interface.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Interface.h"
#include "Screens.h"
#include "../Engine/GameObjectManager.h"

const double Interface::CameraMoveRangex = 0.45;
const double Interface::CameraMoveRangey = 0.45;

void Interface::Load()
{
	Pot = Engine::GetTextureManager().Load("assets/pot.png");
	GoCookingStage = { {{static_cast<double>(Engine::GetWindow().GetSize().x) - Pot->GetSize().x,static_cast<double> (Engine::GetWindow().GetSize().y) - Pot->GetSize().y - 10.0 }
					, {static_cast<double>(Engine::GetWindow().GetSize().x) + Pot->GetSize().x,static_cast<double> (Engine::GetWindow().GetSize().y) + Pot->GetSize().y - 10.0 }}, {0, 0} };
	cameraPos = Vector2D<>{ -700,-700 };
	switch(Engine::GetMap().GetSeason())
	{
		case SPRING:
			Board = Engine::GetTextureManager().Load("assets/spring_board.png");
			break;
		case SUMMER:
			Board = Engine::GetTextureManager().Load("assets/summer_board.png");
			break;
		case FALL:
			Board = Engine::GetTextureManager().Load("assets/fall_board.png");
			break;
		case WINTER:
			Board = Engine::GetTextureManager().Load("assets/winter_board.png");
			break;
	}
	
}

void Interface::Draw()
{
	Pot->Draw(Vector2D <> { static_cast<double>(Engine::GetWindow().GetSize().x) - Pot->GetSize().x, static_cast<double> (Engine::GetWindow().GetSize().y) - Pot->GetSize().y - 10.0 });
	Board->Draw({ static_cast<double>(Engine::GetWindow().GetSize().x) - Board->GetSize().x,static_cast<double> (Engine::GetWindow().GetSize().y) - Pot->GetSize().y - Board->GetSize().y });
}

void Interface::Update()
{
	if (mouseClick.MouseOneClick() && GoCookingStage.isPointInside({ static_cast<double>(doodle::get_mouse_x()), static_cast<double>(doodle::get_mouse_y()) })) {
		Engine::GetGameStateManager().SetNextState(COOKING);
	}
}

void Interface::UnLoad()
{
	Pot = nullptr;
	Board = nullptr;
}

void Interface::CameraUpdate(GameObjectManager& objectManager)
{
	double cameraDistance_x = Engine::GetWindow().GetSize().x * Interface::CameraMoveRangex;
	double cameraDistance_y = Engine::GetWindow().GetSize().y * Interface::CameraMoveRangey;

	if (objectManager.player.GetPosition().x - cameraPos.x > cameraDistance_x) {
		cameraPos.x = objectManager.player.GetPosition().x - cameraDistance_x;
	}
	if (objectManager.player.GetPosition().x - cameraPos.x < cameraDistance_x) {
		cameraPos.x = objectManager.player.GetPosition().x + -cameraDistance_x;
	}
	if (objectManager.player.GetPosition().y - cameraPos.y > cameraDistance_y) {
		cameraPos.y = objectManager.player.GetPosition().y - cameraDistance_y;
	}
	if (objectManager.player.GetPosition().y - cameraPos.y < cameraDistance_y) {
		cameraPos.y = objectManager.player.GetPosition().y + -cameraDistance_y;
	}
	
	CameraLock();
	
	cameraMatrix = TranslateMatrix(-cameraPos);
}

void Interface::CameraLock()
{
	if (cameraPos.x < Engine::GetMap().GetCameraRange().LeftBottom.x)
	{
		cameraPos.x = Engine::GetMap().GetCameraRange().LeftBottom.x;
	}
	if (cameraPos.x > Engine::GetMap().GetCameraRange().TopRight.x)
	{
		cameraPos.x = Engine::GetMap().GetCameraRange().TopRight.x;
	}
	if (cameraPos.y < Engine::GetMap().GetCameraRange().LeftBottom.y)
	{
		cameraPos.y = Engine::GetMap().GetCameraRange().LeftBottom.y;
	}
	if (cameraPos.y > Engine::GetMap().GetCameraRange().TopRight.y)
	{
		cameraPos.y = Engine::GetMap().GetCameraRange().TopRight.y;
	}
}

