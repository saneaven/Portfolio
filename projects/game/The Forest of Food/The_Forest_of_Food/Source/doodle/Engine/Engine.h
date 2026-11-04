/*
File name : Engine.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Kevin Wright,Sieun Choi, Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include <chrono>
#include <random>
#include "Map.h"
#include "GameStateManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "Interpolation.h"
#include "window.h"





class Engine {
public:
	static Engine& Instance() { static Engine instance; return instance; }
	static Input& GetInput() { return Instance().input; }
	static Window& GetWindow() { return Instance().window; }
	static GameStateManager& GetGameStateManager() { return Instance().gameStateManager; }
	static TextureManager& GetTextureManager() { return Instance().texture_manager; }
	static SoundManager& GetSoundManager() { return Instance().sound_manager; }
	static InterpolationManager& GetInterpolationManager() { return Instance().interpolation_manager; }
	static Map& GetMap() { return Instance().map; } 
	 
	static int Random(int min, int max) { return Instance().GetRandomNum(min, max); }

	void Init(std::string windowName);
	void Shutdown();
	void Update();
	bool HasGameEnded();
	int GetRandomNum(int min, int max);
	bool Ending;
	
private:
	Engine();
	~Engine();
	std::chrono::system_clock::time_point lastTick;
	std::chrono::system_clock::time_point point = std::chrono::system_clock::now();
	GameStateManager gameStateManager;
	TextureManager texture_manager;
	SoundManager sound_manager;
	InterpolationManager interpolation_manager;
	Input input;
	Window window;
	Map map;
	std::mt19937 RandomGenerator;
	
	static constexpr double Target_FPS = 30.0;
	double frame_count = 0;
	double total_Frame = 0;
	const double Time_cycle = 5.0;
	double aveFrameRate = 0;
};