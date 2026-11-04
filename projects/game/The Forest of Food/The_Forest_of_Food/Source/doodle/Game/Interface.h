/*
File name : Interface.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/Rectangle.h"
#include "../Engine/Texture.h"
#include "../Engine/MouseClick.h"

class Interface
{
public:
	Interface() :Board(nullptr), Pot(nullptr), GoCookingStage({ {{0,0} ,{0,0}},{0,0} }) {};
	void Load();
	void Draw();
	void Update();
	void UnLoad();
	void CameraUpdate(GameObjectManager&);
	void CameraLock();

	Camera& GetCameraMatrix()
	{
		return cameraMatrix;
	}

private:
	Texture* Board;
	Texture* Pot;
	Rectangle GoCookingStage;
	MouseClick mouseClick;
	Camera cameraMatrix;
	Vector2D<> cameraPos;
	
	static const double CameraMoveRangex;
	static const double CameraMoveRangey;

	
};
