/*
File name : Player.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae kyung
Author(Second):Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/Object.h"
#include "Inventory.h"
#include "Timer.h"
#include "Item.h"

class Player : public Object
{
	Input::InputKey upKey;
	Input::InputKey downKey;
	Input::InputKey leftKey;
	Input::InputKey rightKey;
	Input::InputKey runKey;
	Input::InputKey interactKey;
	Input::InputKey attackKey;

	Inventory* inventory = nullptr;
	Knife knife;
	static const double SoundTime;

	Timer timer;
	double SoundTimer;

	struct Blocked
	{
		bool isUp;
		bool isDown;
		bool isLeft;
		bool isRight;
	};

	Blocked blocked;

	constexpr static double moveAccelValue = 500, maxSpeed = 100;

	void Friction();
	void BorderCheck();
public:
	Player(Vector2D<> position, Vector2D<int> hotspot, std::pair< Vector2D<int>, Vector2D<int> > hitbox, const std::string& filePath, std::string name);

	Player(Player&& player, int) : Player(std::move(player))
	{
		currState = &stateIdle;
		currState->Enter(this);
	}

	Vector2D<>&GetPosition();
	
	class State_Idle : public Object::State
	{
		void Enter(Object* thisObj) override;
		void Update(Object* thisObj, GameObjectManager& objManager, double dt) override;
		void TestForExit(Object* thisObj) override;
		std::string GetName() { return "State_Idle"; }
	};

	class State_Down : public Object::State
	{
		void Enter(Object* thisObj) override;
		void Update(Object* thisObj, GameObjectManager& objManager, double dt) override;
		void TestForExit(Object* thisObj) override;
		std::string GetName() { return "State_Moving"; }
	};
	class State_Up : public Object::State
	{
		void Enter(Object* thisObj) override;
		void Update(Object* thisObj, GameObjectManager& objManager, double dt) override;
		void TestForExit(Object* thisObj) override;
		std::string GetName() { return "State_Moving"; }
	};

	State_Idle stateIdle;
	State_Down stateMovingDown;
	State_Up	stateMovingUp;
	



	void CameraUpdate(Vector2D<>& camera);
	void Update(GameObjectManager& objectManager, double dt) override;

	void CollisionInteractWithEnviroment(Object* obj, Direction direction);
	void CollisionInteractWithItem(Item* obj, Direction direction);
	void Unload() override;
	void Load() override;
	void ResetBlocked();
	void Draw(Camera CameraMatrix);
	void DrawHUD();
	void KeyInput();
	

};
