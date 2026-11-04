/*
File name : GameObjectManager.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Game/Player.h"
#include "../Game/Item.h"
#include "../Game/Creature.h"
#include "../Game/Environment.h"

class GameObjectManager {
public:
	void Load(unsigned int season);
	void Unload();

	void UpdateAll(double dt);
	void DrawAll(TransformMatrix& cameraMatrix, unsigned int season);

	GameObjectManager()noexcept :player(Player({ 0,0 }, { 48, -32 }, { {-20,-40}, {25,30} }, "Character/new_main_character_sprite_sheet.spt", "name")) {}

	Player player;
	Items itemObjects;
	Animals animalObjects;
	Trees treeObjects;
};
