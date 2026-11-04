/*
File name : Player.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae kyung
Author(Second):Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Player.h"
#include "../Engine/GameObjectManager.h"
#include <iostream>
#include "..\Engine/Engine.h"

const double Player::SoundTime = 1.;

Player::Player(Vector2D<> position, Vector2D<int> hotspot, std::pair< Vector2D<int>, Vector2D<int> > hitbox, const std::string& filePath, std::string name)
	: Object(position, hotspot, hitbox, filePath, name, (State*)&stateIdle),
	knife({ 0, 0 }, { 20, -20 }, { { -25,-25 }, { 25, 25 } }, "Character/Stick_s.spt", "Knife"), //need to fix
	upKey(Input::KeyboardButton::Up), downKey(Input::KeyboardButton::Down),
	leftKey(Input::KeyboardButton::Left), rightKey(Input::KeyboardButton::Right),
	runKey(Input::KeyboardButton::Shift), interactKey(Input::KeyboardButton::Control),
	attackKey(Input::KeyboardButton::Space),SoundTimer(SoundTime)
{
	currState->Enter(this);
	sprite.AddHotSpot({ 36,16 });//knife pos
	
}

bool isItemOnTree(ItemName name)
{
	switch (name)
	{
	case ItemName::Enoki:
	case ItemName::Pine:
		return true;
	default:
		return false;
	}
}


void Player::Update(GameObjectManager& objManager, double dt)
{
	if (inventory->ShowInventory() == false) {
		currState->Update(this, objManager, dt);
		sprite.Update(dt);
		position += velocity * dt;
		
		knife.Update(objManager, dt);
	}
	
	timer.Update(dt);

	Direction temp;

	bool isCollide = false;
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
			isCollide = true;
			std::cout << "Direction : " << temp << std::endl;
			std::cout << "[Tree] \n";
		}
	}
	if (!isCollide)
	{
		ResetBlocked();
	}

	std::vector<Item*> destroyList;
	for (auto i : objManager.itemObjects.items)
	{
		temp = CollisionCheck(i, dt);
		if (temp != Direction::notD && interactKey.IsKeyDown() && InventoryItems.size() < 32)
		{
			if (isItemOnTree(i->GetItemName()))
				objManager.treeObjects.GetTree().push_back(new Environment{ i->GetPosition(),{ {10,10},{110,110} },"Map_Design/Winter_tree_s.spt" });

			CollisionInteractWithItem(i, temp);
			destroyList.push_back(i);

			sprite.PlayAnimation(3);

			std::cout << "[Item] \n";
		}
	}

	for (auto i : destroyList)
	{
		objManager.itemObjects.items.remove(i);
		delete i;
	}


	HitboxUpdate();
	BorderCheck();
	currState->TestForExit(this);
	inventory->Update();

	objectMatrix = TranslateMatrix(position);
}

void Player::BorderCheck()
{
	
	if (position.x  < Engine::GetMap().GetMap_Size().LeftBottom.x)
		position.x = Engine::GetMap().GetMap_Size().LeftBottom.x + sprite.GetFrameSize().x/2;
	if (position.x > Engine::GetMap().GetMap_Size().TopRight.x)
		position.x = Engine::GetMap().GetMap_Size().TopRight.x - sprite.GetFrameSize().x/2;
	if (position.y < Engine::GetMap().GetMap_Size().LeftBottom.y)
		position.y = Engine::GetMap().GetMap_Size().LeftBottom.y ;
	if (position.y > Engine::GetMap().GetMap_Size().TopRight.y)
		position.y = Engine::GetMap().GetMap_Size().TopRight.y;

}


void Player::Load()
{
	ResetBlocked();
	position = { 0,0 };
	timer.Load();
	inventory = new Inventory;
	if(InventoryItems.size() > 0){
		inventory->AddItem(new Item({ 0, 0 }, { 0, 0 }, "cooking/ramen_s.spt", "Soup base", ItemName::Ramen, { 0, 0 }, { 0,  0 }));
	}
	AddSFX("walk", "sounds/walk.ogg");
	SoundTimer = SoundTime;
}
void Player::Unload()
{
	Object::Unload();
	knife.Unload();
	inventory = nullptr;
}

void Player::ResetBlocked()
{
	blocked.isUp = false;
	blocked.isDown = false;
	blocked.isRight = false;
	blocked.isLeft = false;
}

Vector2D<>& Player::GetPosition()
{
	return position;
}


void Player::CameraUpdate(Vector2D<>& camera)
{
	camera = -position + (Engine::GetWindow().GetSize() / 2);

}

void Player::Draw(Camera camera)
{
	Object::Draw(camera);
	//HitboxDraw(camera);

	knife.Draw(camera);
#if _DEBUG
	//std::cout << "Player :" << std::endl;
	//std::cout << position << std::endl;
	//std::cout << velocity << std::endl;
	system("cls");
#endif
}

void Player::DrawHUD()
{
	timer.Draw();
	inventory->Draw();
}

void Player::Friction()
{
	if (runKey.IsKeyDown())
	{
		if (velocity.GetLength() > maxSpeed * 2)
		{
			velocity.SetLength(maxSpeed * 2);
		}
	}
	else if (velocity.GetLength()> maxSpeed)
		velocity.SetLength(maxSpeed);

}

void Player::KeyInput()
{
	velocity = { 0,0 };

	if (upKey.IsKeyDown() && !blocked.isUp)
	{
		velocity += {0, maxSpeed};
		blocked.isDown = false;
		
	}
	if (downKey.IsKeyDown() && !blocked.isDown)
	{
		velocity += {0, -maxSpeed};
		blocked.isUp = false;
		
	}
	if (rightKey.IsKeyDown() && !blocked.isRight)
	{
		velocity += {maxSpeed, 0};
		blocked.isLeft = false;
		
		
	}
	if (leftKey.IsKeyDown() && !blocked.isLeft)
	{
		velocity += {-maxSpeed, 0};
		blocked.isRight = false;
		
		
	}
	if (runKey.IsKeyDown())
	{
		velocity *= 2.;
	}
	if (attackKey.IsKeyRelease())
	{
		knife.useKnife();
	}
	
	//if (interactKey.IsKeyDown())
	//{
	//	sprite.PlayAnimation(3);
	//}
}

void Player::CollisionInteractWithEnviroment(Object* obj, Direction direction)
{
	constexpr double correction = 2.;
	switch (direction)
	{
	case Direction::up:
		position.y += obj->GetHitbox().GetPositionOf(Direction::up).y - GetHitbox().GetPositionOf(Direction::down).y + correction;
		velocity.y = 0;
		blocked.isDown = true;
		break;
	case Direction::down:
		position.y += obj->GetHitbox().GetPositionOf(Direction::down).y - GetHitbox().GetPositionOf(Direction::up).y - correction;
		velocity.y = 0;
		blocked.isUp = true;
		break;
	case Direction::left:
		position.x += obj->GetHitbox().GetPositionOf(Direction::left).x - GetHitbox().GetPositionOf(Direction::right).x - correction;
		velocity.x = 0;
		blocked.isRight = true;
		break;
	case Direction::right:
		position.x += obj->GetHitbox().GetPositionOf(Direction::right).x - GetHitbox().GetPositionOf(Direction::left).x + correction;
		velocity.x = 0;
		blocked.isLeft = true;
		break;
	}
}

void Player::CollisionInteractWithItem(Item* obj, Direction )
{
	inventory->AddItem(obj);
}

void Player::State_Idle::Enter(Object* thisObj)
{
	Player* player = static_cast<Player*>(thisObj);
	player->sprite.PlayAnimation(0);
	player->knife.isFlipped = true;
	
	
}

void Player::State_Idle::Update(Object* thisObj, GameObjectManager&, double dt)
{
	Player* player = static_cast<Player*>(thisObj);
	
	player->KeyInput();

	player->velocity += player->acceleration * dt;
	
	player->Friction();



}

void Player::State_Idle::TestForExit(Object* thisObj)
{
	Player* player = static_cast<Player*>(thisObj);

	if (player->velocity != Vector2D{0., 0.})
	{
		player->ChangeState(&player->stateMovingDown);
	}
}


void Player::State_Down::Enter(Object*object)
{
	Player* player = static_cast<Player*>(object);

	player->sprite.PlayAnimation(1);

	player->knife.isFlipped = true;


}

void Player::State_Down::Update(Object* thisObj, GameObjectManager&, double dt)
{
	Player* player = static_cast<Player*>(thisObj);
	
	player->KeyInput();
	
	player->Friction();

	player->velocity += player->acceleration * dt;

	if (player->SoundTimer <= 0)
	{
		player->PlaySFX("walk");
		player->SoundTimer = SoundTime;
	}
	player->SoundTimer -= dt;

	
}

void Player::State_Down::TestForExit(Object* thisObj)
{
	Player* player = static_cast<Player*>(thisObj);

	if (player->velocity == Vector2D{ 0., 0. })
	{
		player->ChangeState(&player->stateIdle);
		
	}
	if(player->upKey.IsKeyDown() == true)
	{
		player->ChangeState(&player->stateMovingUp);
	}


}
void Player::State_Up::Enter(Object* object)
{
	Player* player = static_cast<Player*>(object);

	player->sprite.PlayAnimation(2);

	player->knife.isFlipped = false;
}

void Player::State_Up::Update(Object* thisObj, GameObjectManager&, double dt)
{
	Player* player = static_cast<Player*>(thisObj);

	player->KeyInput();

	player->Friction();

	player->velocity += player->acceleration * dt;

	if (player->SoundTimer <= 0)
	{
		player->PlaySFX("walk");
		player->SoundTimer = SoundTime;
	}
	player->SoundTimer -= dt;
}

void Player::State_Up::TestForExit(Object* thisObj)
{
	Player* player = static_cast<Player*>(thisObj);

	if (player->velocity == Vector2D{ 0., 0. })
	{
		player->ChangeState(&player->stateIdle);

	}


}
