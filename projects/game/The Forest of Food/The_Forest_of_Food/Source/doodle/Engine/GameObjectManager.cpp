/*
File name : GameObjectManager.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "GameObjectManager.h"

void GameObjectManager::Load(unsigned int season)
{
	animalObjects.Load(season);
	treeObjects.Load(season);
	itemObjects.Load(season);
	player.Load();
}
void GameObjectManager::UpdateAll(double dt)
{
	itemObjects.Update(*this, dt);
	animalObjects.Update(*this, dt);
	treeObjects.Update(*this, dt);
	player.Update(*this, dt);
}
void GameObjectManager::DrawAll(TransformMatrix& cameraMatrix, unsigned int season)
{
	itemObjects.Draw(cameraMatrix, season);
	animalObjects.Draw(cameraMatrix);

	auto treeAfterPlayer = std::find_if(begin(treeObjects.trees), end(treeObjects.trees),
		[this](Environment* elem)
		{
			return this->player.GetPosition().y > elem->GetPosition().y;
		});
	for_each(begin(treeObjects.trees), treeAfterPlayer,
		[cameraMatrix](Environment* elem)
		{
			elem->Draw(cameraMatrix);
		});
	player.Draw(cameraMatrix);
	for_each(treeAfterPlayer, end(treeObjects.trees),
		[cameraMatrix](Environment* elem)
		{
			elem->Draw(cameraMatrix);
		});
	player.DrawHUD();
}

void GameObjectManager::Unload()
{
	player.Unload();
	itemObjects.UnLoad();
	animalObjects.Unload();
	treeObjects.Unload();

}


