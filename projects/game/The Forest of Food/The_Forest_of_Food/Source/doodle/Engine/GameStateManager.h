/*
File name : GameStateManager.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "GameState.h"
#include "Input.h"

class GameStateManager {
public:
	GameStateManager();

	void AddGameState(GameState& gameState);
	void Update(double dt);
	void SetNextState(int initState);
	void Shutdown();
	void ReloadState();
	bool HasGameEnded() { return state == State::EXIT; }
	Input::InputKey Exit{ Input::KeyboardButton::Escape };

private:
	enum class State {
		START,
		LOAD,
		RUNNING,
		UNLOAD,
		SHUTDOWN,
		EXIT,
	};

	std::vector<GameState*> gameStates;
	State state;
	GameState* currGameState;
	GameState* nextGameState;
};

