/*
File name : GameState.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GameState.h"
#include "Sound.h"

void GameState::SetBGM(std::string filePath)
{
	bgm = new BGM{ filePath };
	bgm->Play();

}
void GameState::UnloadBGM()
{
	bgm->Stop();
	delete bgm;
}