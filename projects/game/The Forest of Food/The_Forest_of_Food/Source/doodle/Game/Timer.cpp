/*
File name : Timer.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Timer.h"
#include "..\Engine/Engine.h"
#include <iostream>
#include "Screens.h"

Timer::Timer() : minute(5), second(0), WindowY(Engine::GetWindow().GetSize().y), WindowX(Engine::GetWindow().GetSize().x), TimeRect((static_cast<double>(Engine::GetWindow().GetSize().x) / 2) / 300.0), OneSecond(0)
, timer("assets/timer.png", { static_cast<double>(Engine::GetWindow().GetSize().x) /2 -75, static_cast<double>(Engine::GetWindow().GetSize().y)-50 }) {
}


void Timer::Load() {
	minute = 5;
	second = 0;
	TimeRect += (static_cast<double>(Engine::GetWindow().GetSize().x) - 100) / 300.0;
	timer.SetPos({ static_cast<double>(Engine::GetWindow().GetSize().x) / 2 - 75, static_cast<double>(Engine::GetWindow().GetSize().y) - 50 });
}


void Timer::Draw() {
	doodle::push_settings();
	doodle::no_outline();
	doodle::set_font_size(32);
	doodle::set_font(pixelFontStyle);


	timer.Draw({ static_cast<double>(Engine::GetWindow().GetSize().x) / 2 - 75, static_cast<double>(Engine::GetWindow().GetSize().y) - 50 });
	doodle::set_fill_color(doodle::Color{ 0 });
	std::cout.precision(2);
	doodle::draw_text(std::to_string(minute) + " : " + std::to_string(static_cast<int>(second)), static_cast<double>(Engine::GetWindow().GetSize().x) / 2 - 40, static_cast<double>(Engine::GetWindow().GetSize().y) - 40);
	doodle::pop_settings();
}



void Timer::Update(double dt) {
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	if(second <= 0) {
		--minute;
		second = 60.0;
	}
	second -= dt;
	OneSecond += dt;
	if(OneSecond >= 1) {
		TimeRect += (WindowX - 100) / 300.0;
		OneSecond = 0;
	}
	if(GetTimeMin() <= 0 && GetTimeSec() <= 0){
		Engine::GetGameStateManager().SetNextState(COOKING);
	}
}


void Timer::UnLoad() {
	
}

double Timer::GetTimeMin() {
	return minute;
}

double Timer::GetTimeSec() {
	return second;
}
