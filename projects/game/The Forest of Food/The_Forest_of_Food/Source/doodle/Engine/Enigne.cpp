/*
File name : Engine.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Kevin Wright,Sieun Choi, Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/



#include "Engine.h"

Engine::Engine() : gameStateManager(),Ending(false){}

Engine::~Engine() {
}


void Engine::Init(std::string window_name) {

	
	Engine::GetWindow().Init(window_name);
	lastTick = std::chrono::system_clock::now();
	std::random_device randomSeed;
	std::mt19937 temp(randomSeed());
	RandomGenerator = temp;
}

void Engine::Update() {

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	double dt = std::chrono::duration<double>(now - lastTick).count();

	if (std::chrono::duration<double>(now - point).count() >= Time_cycle) {
		aveFrameRate = total_Frame / frame_count;
		total_Frame = 0;
		frame_count = 0;
		point = std::chrono::system_clock::now();
	}

	if (dt >= 1 / Engine::Target_FPS) {
#ifdef _DEBUG
		GetGameStateManager().Update(0.0333);
		GetInterpolationManager().Update(0.0333);
#else
		GetGameStateManager().Update(dt);
		GetInterpolationManager().Update(dt);
#endif
		GetInput().Update();
		GetWindow().Update();
		lastTick = now;
		frame_count++;
		total_Frame += 1 / dt;
	}

}

void Engine::Shutdown() {


	Engine::GetGameStateManager().Shutdown();
	


}
bool Engine::HasGameEnded() {


	return Engine::GetGameStateManager().HasGameEnded();

}

std::ostream& operator<<(std::ostream& output, const Direction direction)
{
	switch (direction)
	{
	case Direction::notD:
		output << "notD";
		break;
	case Direction::center:
		output << "center";
		break;
	case Direction::up:
		output << "up";
		break;
	case Direction::down:
		output << "down";
		break;
	case Direction::left:
		output << "left";
		break;
	case Direction::right:
		output << "right";
		break;
	case Direction::rightUp:
		output << "rightUp";
		break;
	case Direction::rightDown:
		output << "rightDown";
		break;
	case Direction::leftDown:
		output << "leftDown";
		break;
	}

	return output;
}

int Engine::GetRandomNum(int min, int max)
{
	std::uniform_int_distribution<> range(min, max);

	return range(RandomGenerator);
}