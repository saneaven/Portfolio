/*
File name : GameStateManager.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(Second) : Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Engine.h"
#include "GameStateManager.h"
#include "doodle/doodle.hpp"


GameStateManager::GameStateManager() : currGameState(nullptr), nextGameState(nullptr), state(State::START) { }

void GameStateManager::AddGameState(GameState& gameState) {
	gameStates.push_back(&gameState);
}


void GameStateManager::Update(double dt) {

	switch (state)
	{
	case State::START:
		nextGameState = gameStates[0];
		state = State::LOAD;
		break;

	case State::LOAD:

		currGameState = nextGameState;
		currGameState->Load();
		
		state = State::RUNNING;
		break;

	case State::RUNNING:

		if (currGameState == nextGameState)
		{
			currGameState->Update(dt);
			currGameState->Draw();
	
			if (Exit.IsKeyRelease() == true) {
				state = State::SHUTDOWN;
				doodle::close_window();
			}
		}
		else
		{
			state = State::UNLOAD;
		}
		break;

	case State::UNLOAD:

		currGameState->UnLoad();
		if (nextGameState != nullptr)
		{
			state = State::LOAD;
		}
		else if(nextGameState == nullptr)
		{
			state = State::SHUTDOWN;
		}
		break;

	case State::SHUTDOWN:
		state = State::EXIT;
		break;
	default:
		break;


	}
}

void GameStateManager::SetNextState(int initState) {
	nextGameState = gameStates[initState];
}

void GameStateManager::ReloadState() {
	state = State::UNLOAD;
}

void GameStateManager::Shutdown() {
	state = State::UNLOAD;
	nextGameState = nullptr;
}
