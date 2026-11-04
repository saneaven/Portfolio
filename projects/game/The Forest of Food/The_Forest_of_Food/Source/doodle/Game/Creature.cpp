/*
File name : Creature.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
Author(Second) : MinJae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Creature.h"
#include "..\Engine\Engine.h"
#include "../Engine/GameObjectManager.h"
#include "Screens.h"

constexpr double Creature::speed = 100;
constexpr double Creature::waitTime = 1;
constexpr double Creature::invincibleTime = 1;
constexpr double Creature::SoundTime = 15.;



Creature::Creature(Vector2D<> pos,Vector2D<int>hotspot,std::pair< Vector2D<int>, Vector2D<int> > h, const std::string& filePath,int code)
	:Object(pos, h, hotspot, filePath), hp(4),IsFlipped(true), IsAniamlDead(false),Meat(nullptr),SoundTimer(0),nowInvincibleTime(0),nowTurnTime(0),nowWaitTime(0)
,unique_code(code),turnTime(0){
	acceleration = {8,0};
}

void Creature::Load() {
	
	currState = &moving;
	currState->Enter(this);
	nowWaitTime = 0;
	velocity = { speed,0 };
	hp = 2;
	turnTime = Engine::Random(5, 10);
	nowTurnTime = turnTime;
	nowInvincibleTime = 0;
	SoundTimer = SoundTime;
	switch (unique_code)
	{
	case static_cast<int>(AnimalCode::COW):
		AddSFX("Animal", "sounds/cow.ogg");
		break;
	case static_cast<int>(AnimalCode::CHICKEN) :
		AddSFX("Animal", "sounds/chicken.wav");
		break;
	case static_cast<int>(AnimalCode::TURKEY) :
		AddSFX("Animal", "sounds/chicken.wav");
		break;
	case static_cast<int>(AnimalCode::PIG) :
		AddSFX("Animal", "sounds/pig.ogg");
		break;
	}
	PlaySFX("Animal");
}

void Creature::Update(GameObjectManager& objManager, double dt)
{
	nowInvincibleTime -= dt;
	Animal_Lock();
	
	currState->Update(this, objManager, dt);

	HitboxUpdate();
	
	sprite.Update(dt);

	currState->TestForExit(this);
	
	Direction temp;
	for (auto i : objManager.treeObjects.trees)
	{
		temp = CollisionCheck(i, dt);
		if (temp == Direction::center)
		{
			temp = i->CollisionCheck(this, dt);
			temp = reverseDirection(temp);
		}
		if (temp != Direction::notD)
		{
			CollisionInteractWithEnviroment(i, temp);
			std::cout << "Direction : " << temp << std::endl;
			std::cout << "[Tree] \n";
		}
	}

	objectMatrix = TranslateMatrix(position);
	
	if (velocity.x > 0) 
	{
		objectMatrix *= ScaleMatrix({ -1.0, 1.0 });
	}
	else if (velocity.x > 0)
	{
		objectMatrix *= ScaleMatrix({ 1.0,1.0 });
	} //if velocity.x == 0 , just maintain previous matrix
	
}
//State Function

void Creature::State_Moving::Enter(Object* thisObj)
{
	Creature* c = static_cast<Creature*>(thisObj);
	c->sprite.PlayAnimation(0);
	
}
void Creature::State_Moving::Update(Object* thisObj, GameObjectManager&, double dt)
{
	Creature* c = static_cast<Creature*>(thisObj);
	c->Animal_Move(dt);
	c->Animal_Lock();
	

	if (c->SoundTimer <= 0)
	{
		c->PlaySFX("Animal");
		c->SoundTimer = SoundTime;
	}
	c->SoundTimer -= dt;
	
}
void Creature::State_Moving::TestForExit(Object* thisObj)
{
	Creature* c = static_cast<Creature*>(thisObj);
	
	if(c->IsAniamlDead == true)
	{
		c->ChangeState(&c->dying);
	}
}

void Creature::State_Runaway::Enter(Object* thisObj)
{
	Creature* c = static_cast<Creature*>(thisObj);
	c->sprite.PlayAnimation(1);
}
void Creature::State_Runaway::Update(Object* thisObj, GameObjectManager& objManager, double dt)
{
	Creature* c = static_cast<Creature*>(thisObj);
	UpdatePlayerPosition(objManager.player.GetPosition());
 	c->Animal_Runaway(playerPosition, dt);
	c->Animal_Lock();

}
void Creature::State_Runaway::TestForExit(Object*)
{
}

void Creature::State_Runaway::UpdatePlayerPosition(Vector2D<> pos)
{
	playerPosition = pos;
}

void Creature::State_Dying::Enter(Object*)
{
	//Dying Animation play
	
}
void Creature::State_Dying::Update(Object* , GameObjectManager&, double)
{
	
}
void Creature::State_Dying::TestForExit(Object* )
{
	
}
void Creature::Draw(Camera CameraMatrix)
{
	if (IsAniamlDead == false) {
		
		Object::Draw(CameraMatrix);
		
	}

	
}

//Helper Function
void Creature::CollisionInteractWithKnife(GameObjectManager& objManager, double)
{
	if (nowInvincibleTime <= 0)
	{
		--hp;
		if (hp == 0)
		{
			ChangeState(&dying);
			DeadFunction(objManager, this);
		}
		else
		{
			ChangeState(&runaway);
		}
		nowInvincibleTime = invincibleTime;
	}
}

void Creature::CollisionInteractWithEnviroment(Object* obj, Direction direction)
{
	constexpr double correction = 5.;
	switch (direction)
	{
	case Direction::up:
		position.y += obj->GetHitbox().GetPositionOf(Direction::up).y - GetHitbox().GetPositionOf(Direction::down).y + correction;
		velocity.y = -velocity.y;
		break;
	case Direction::down:
		position.y += obj->GetHitbox().GetPositionOf(Direction::down).y - GetHitbox().GetPositionOf(Direction::up).y - correction;
		velocity.y = -velocity.y;
		break;
	case Direction::left:
		position.x += obj->GetHitbox().GetPositionOf(Direction::left).x - GetHitbox().GetPositionOf(Direction::right).x - correction;
		velocity.x = -velocity.x;
		break;
	case Direction::right:
		position.x += obj->GetHitbox().GetPositionOf(Direction::right).x - GetHitbox().GetPositionOf(Direction::left).x + correction;
		velocity.x = -velocity.x;
		break;
	}
}

void Creature::Animal_Move(double dt)
{

	if (nowTurnTime <= 0)
	{
		IsFlipped = !IsFlipped;
		velocity = -velocity;
		nowTurnTime = turnTime;
		nowWaitTime = waitTime;
	}

	if (nowWaitTime > 0)
	{
		nowWaitTime -= dt;
	}
	else
	{
		nowTurnTime -= dt;
		position += velocity * dt;
	}
}

void Creature::Animal_Runaway(Vector2D<> playerPos, double dt)
{
	Vector2D<> RunDirection = (position -  playerPos).Normalize();
	velocity = RunDirection * (speed * 2.1);
	position += velocity * dt;
}

void Creature::Animal_Lock()
{
	if(position.x >= Engine::GetMap().GetMap_Size().TopRight.x- sprite.GetFrameSize().x/2)
	{
		position.x = Engine::GetMap().GetMap_Size().TopRight.x - sprite.GetFrameSize().x / 2;
	}
	if (position.x <= Engine::GetMap().GetMap_Size().LeftBottom.x + sprite.GetFrameSize().x / 2)
	{
		position.x = Engine::GetMap().GetMap_Size().LeftBottom.x + sprite.GetFrameSize().x / 2;
	}
	if (position.y >= Engine::GetMap().GetMap_Size().TopRight.y - sprite.GetFrameSize().y / 2)
	{
		position.y = Engine::GetMap().GetMap_Size().TopRight.y - sprite.GetFrameSize().y / 2;
	}
	if (position.y <= Engine::GetMap().GetMap_Size().LeftBottom.y + sprite.GetFrameSize().y / 2)
	{
		position.y = Engine::GetMap().GetMap_Size().LeftBottom.y + sprite.GetFrameSize().y / 2;
	}
}

void Creature::DeadFunction(GameObjectManager& o, Creature* cr)
{
	Vector2D<> P = { 8000,8000 };
	cr->IsAniamlDead = true;
	cr->velocity = { 0,0 };

	
	Vector2D<> Animal_Pos = { position.x,position.y };
	if (unique_code == static_cast<int>(AnimalCode::COW)) {
		Meat = (new Item({ Animal_Pos }, { 0, 0 }, "assets/beef_s.spt", "Beef", ItemName::Beef, { 6, 14 }, { 89, 77 }));
	}
	if (unique_code == static_cast<int>(AnimalCode::CHICKEN)) {
		Meat = (new Item({ Animal_Pos }, { 0, 0 }, "assets/chicken_s.spt", "Chicken", ItemName::Chicken, {5, 22}, {92, 69}));
	}
	if (unique_code == static_cast<int>(AnimalCode::TURKEY)) {
		Meat = (new Item({ Animal_Pos }, { 0, 0 }, "assets/turkey_s.spt", "Turkey", ItemName::Turkey, {5, 22}, {92, 69}));
	}
	if (unique_code == static_cast<int>(AnimalCode::PIG)) {
		Meat = (new Item({ Animal_Pos }, { 0, 0 }, "assets/pork_s.spt", "Pork", ItemName::Pork, {8, 10}, {86, 78}));
	}

	o.itemObjects.AddItems(Meat);
	cr->position = { P };

}



void Animals::Load(unsigned int season)
{
	switch (season)
	{
	case SPRING:
	{
		Set_Position("animal/cow_s.spt", static_cast<int>(AnimalCode::COW));
			
		for (Creature* c : animals)
		{
			c->Load();
		}
			
		break;
	}
	case SUMMER:
	{
		p = "animal/chicken_s.spt";
		Set_Position(p, static_cast<int>(AnimalCode::CHICKEN));
		for (Creature* c : animals)
		{
			c->Load();
		}
		break;
	}
	case FALL:
	{
		p = "animal/turkey_s.spt";
		Set_Position(p, static_cast<int>(AnimalCode::TURKEY));
		for (Creature* c : animals)
		{
			c->Load();
		}
		break;
	}
	case WINTER:
	{
		p = "animal/pig_s.spt";
		Set_Position(p, static_cast<int>(AnimalCode::PIG));
		for (Creature* c : animals)
		{
			c->Load();
		}
		break;
	}
	}
}


void Animals::Update(GameObjectManager& objectManager, double dt)
{
	for (Creature* c : animals)
	{
		c->Update(objectManager, dt);
	}

}

void Animals::Draw(Camera CameraMatrix)
{
	for(Creature* c:animals)
	{
		c->Draw(CameraMatrix);
	}
	
}

void Animals::Set_Position(std::string path,int keycode)
{
	
	for(int i = 0;i<5;i++)
	{
		animals.emplace_back(new Creature({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos }, { 128,16 }, { {0,0},Vector2D<>{150.0,100.0} }, path, keycode));
	}
	//For Egg Spawn
	if(Engine::GetMap().GetSeason() == SPRING)
	animals.emplace_back(new Creature({ Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos }, { 128,16 }, { {0,0},Vector2D<>{150.0,100.0} }, "animal/chicken_s.spt",static_cast<int>(AnimalCode::CHICKEN)));
}



void Animals::Unload()
{
	
	for (Creature* c : animals)
	{
		c->Unload();
		delete c;
	}
	animals.clear();
}

void Animals::Animal_Erase(Creature* erase,GameObjectManager& o)
{

	erase->DeadFunction(o,erase);
	
}





