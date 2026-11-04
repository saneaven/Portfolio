/*
File name : Creature.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
Author(Second) : MinJae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/Object.h"
#include "Item.h"

enum class AnimalCode
{
	COW,
	CHICKEN,
	TURKEY,
	PIG
};

class Creature : public Object
{

private:
	unsigned int hp;
	bool IsFlipped;
	double turnTime, nowTurnTime, nowWaitTime, nowInvincibleTime,SoundTimer;
	const static double speed, waitTime, invincibleTime, SoundTime;
	int unique_code;
	

public:
	bool IsAniamlDead;
	
	class State_Moving : public Object::State
	{
		void Enter(Object* thisObj) override;
		void Update(Object* thisObj, GameObjectManager& objManager, double dt) override;
		void TestForExit(Object* thisObj) override;
		std::string GetName() { return "State_Moving"; }
	};

	class State_Runaway : public Object::State
	{
		void Enter(Object* thisObj) override;
		void Update(Object* thisObj, GameObjectManager& objManager, double dt) override;
		void TestForExit(Object* thisObj) override;
		std::string GetName() { return "State_Runaway"; }

	public:
		void UpdatePlayerPosition(Vector2D<> pos);
		Vector2D<> playerPosition;
	};

	class State_Dying : public Object::State
	{
		void Enter(Object* thisObj) override;
		void Update(Object* thisObj, GameObjectManager& objManager, double dt) override;
		void TestForExit(Object* thisObj) override;
		std::string GetName() { return "State_Dying"; }
	};
	
	Creature(Vector2D<> pos, Vector2D<int>hotspot,std::pair< Vector2D<int>, Vector2D<int> > hitbox, const std::string& filePath,int code);
	void Load();

	void Update(GameObjectManager& objectManager, double dt) override;
	void Animal_Runaway(Vector2D<> playPos, double dt);
	void Animal_Move(double);
	void Animal_Lock();
	void DeadFunction(GameObjectManager&,Creature*);
	const unsigned int& GetHp()const { return this->hp; }
	void Decrement_Hp() { hp--; }
	void Draw(Camera CamerMatrix);//Camera Range Version
	void CollisionInteractWithKnife(GameObjectManager& objManager, double dt);
	void CollisionInteractWithEnviroment(Object* obj, Direction direction);
	~Creature() = default;
private:
	State_Moving moving;
	State_Runaway runaway;
	State_Dying dying;
	Item* Meat;
	
	
	

};


class Animals
{
private:
	
	std::vector<Creature*>animals;
	std::string p;
	
	
public:

	void Load(unsigned int season);
	void Update(GameObjectManager& objectManager, double dt);
	void Set_Position(std::string,int);//random value
	void Draw(Camera CamerMatrix);
	void Animal_Erase(Creature*, GameObjectManager&);
	
	std::vector<Creature*>&GetAnimals()
	{return animals;}
	~Animals()
	{
		animals.clear();
	};
	void Unload();
	
};