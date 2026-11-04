/*
File name : Environment.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "../Engine/Object.h"


enum Non_Creature
{
	SPRING_TREE,
	SUMMER_TREE,
	FALL_TREE,
	WINTER_TREE
};

class Environment :public Object
{

private:
	

public:
	Environment(Vector2D<>pos, std::pair< Vector2D<int>, Vector2D<int> > hitbox, const std::string& filePath):
	Object(pos,hitbox,Vector2D<>{8.0,16.0},filePath)
	{
		
	};
	
	
	void Load();
	void Update(GameObjectManager&, double) override;
	void Draw();
	void Draw(Camera cameraMatrix);
	
};

class Trees
{
private:
	
	std::string p_ ;
	
	
public:
	std::vector<Environment*>trees;

	void Update(GameObjectManager&, double);
	void Draw(Camera);
	void Load(unsigned int season);
	void Unload();
	void Set_Spawn(std::string& path);
	std::vector<Environment*>& GetTree()
	{
		return trees;
	}
	~Trees()
	{
		trees.clear();
	};

	
};