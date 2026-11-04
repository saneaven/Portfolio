/*
File name : Environment.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
Author(Second) : MinJae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Environment.h"
#include <algorithm>
#include "../Engine/Engine.h"
#include "Screens.h"


void Environment::Load()
{
}

void Environment::Update(GameObjectManager&, double)
{
//Collision Check
	HitboxUpdate();

	objectMatrix = TranslateMatrix(position);
	
}

void Environment::Draw(Camera cameraMatrix)
{
	Object::Draw(cameraMatrix);
	//Object::HitboxDraw(cameraMatrix);
}

void Trees::Load(unsigned int season)
{
	switch(season)
	{
	case SPRING:
		{
		p_ = "Map_Design/Spring_tree_s.spt";
		Set_Spawn(p_);
		break;
		}
	case SUMMER:
	{
		p_ = "Map_Design/Summer_tree_s.spt";
		Set_Spawn(p_);
		break;
	}
	case FALL:
	{
		p_ = "Map_Design/Fall_tree_s.spt";
		Set_Spawn(p_);
		break;
	}
	case WINTER:
	{
		p_ = "Map_Design/Winter_tree_s.spt";
		Set_Spawn(p_);
		break;
	}
	}

}

void Trees::Update(GameObjectManager& objectManager, double dt)

{
	
	for (Environment* e : trees)
	{
		e->Update(objectManager, dt);
	}
}


void Trees::Set_Spawn(std::string& path)
{
	
	for (int i = 0;i < 100;i++) 
	{
		trees.emplace_back(new Environment({Engine::GetMap().Get_Map_Info(Engine::GetMap().Random_Number()).pixel_pos }, { {10,10},{110,110} }, path));
	}
	std::sort(begin(trees), end(trees), 
		[](Environment* elem1, Environment* elem2) 
		{
			return elem1->GetPosition().y > elem2->GetPosition().y;
		});
}



void Trees::Draw(Camera cameraMatrix)
{
	for (Environment* e : trees)
	{
		e->Draw(cameraMatrix);
	}

}

void Trees::Unload()
{
	for(Environment* e:trees)
	{
		e->Unload();
		delete e;
	}
	
	trees.clear();
	
}



