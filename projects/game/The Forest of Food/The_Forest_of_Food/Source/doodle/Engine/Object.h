/*
File name : Object.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Sprite.h"
#include "Rectangle.h"
#include "Transform.h"
#include "Sound.h"
#include <unordered_map>

using Camera = TransformMatrix;

class GameObjectManager;

class Object
{
	std::unordered_map<std::string, SFX*> soundEffects;
	Rectangle hitbox;

	std::string name;

protected:
	Sprite sprite;
	Vector2D<> position;
	Vector2D<> velocity;
	Vector2D<> acceleration;
	TransformMatrix objectMatrix;

	class State
	{
	public:
		virtual void Enter(Object* thisObj) = 0;
		virtual void Update(Object* thisObj, GameObjectManager& objManager, double dt) = 0;
		virtual void TestForExit(Object* thisObj) = 0;
		virtual std::string GetName() = 0;
	};

	State* currState;

	void ChangeState(State* state)
	{
		currState = state;
		currState->Enter(this);
	}


	virtual void CollisionInteractWith(Object*, Direction);
	void SetName(std::string name);

	std::string GetName();
	

	void HitboxDraw(Camera CameraMatrix);
	void HitboxUpdate();

	void AddSFX(std::string name,std::string filePath);
	void PlaySFX(std::string name);
	void RePlaySFX(std::string name);


public:

	//FOR ITEM
	Object(Vector2D<> position, Vector2D<int> hotspot, std::pair< Vector2D<int>, Vector2D<int> > hitbox
		, const std::string& filePath, std::string name)
		: position(position), velocity({ 0,0 }), hitbox(hitbox, hotspot), name(name), currState(nullptr)
	{
		sprite.Load(filePath);
	};
	//For TREE,ANIMAL
	Object(Vector2D<> position, std::pair< Vector2D<int>, Vector2D<int> > hitbox, Vector2D<int> hotspot, const std::string& filePath)
		: position(position), velocity({ 0,0 }), hitbox(hitbox, hotspot), currState(nullptr),objectMatrix(TranslateMatrix(position))
	{
		sprite.Load(filePath);
	};
	//FOR PLAYER
	Object(Vector2D<> position, Vector2D<int> hotspot, std::pair< Vector2D<int>, Vector2D<int> > hitbox
		, const std::string& filePath, std::string name, State* initState)
		: position(position), velocity({ 0,0 }), hitbox(hitbox, hotspot),name(name), currState(initState)
	{
		sprite.Load(filePath);
	};

	Direction CollisionCheck(const Object* other, double dt) const;

	const Rectangle& GetHitbox() const;
	Vector2D<int> GetHotSpot(int i);
	Vector2D<> GetPosition() { return position; };

	virtual void Update(GameObjectManager& objectManager, double dt) = 0;
	void Draw(Camera CameraMatrix);

	virtual void Load() {}
	virtual void Unload();
};

