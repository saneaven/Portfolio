/*
File name : AnimationTimer.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#include <doodle/doodle.hpp>
#include <vector>
#include <functional>
#include "basicFunc.h"
#include "classes.h"
using namespace std;


struct Timer
{
	function<void()> trackFunction;
	int Latency = 0;

	Timer(function<void()> func, int ms);
};
Timer::Timer(function<void()> func, int ms) : trackFunction(func), Latency(ms) {};


vector<Timer> timers;

void timerUpdate()
{
	int deltaMillisec = static_cast<int>(doodle::DeltaTime * 1000);
	for (int i = 0; i < timers.size(); ++i)
	{
		timers[i].Latency -= deltaMillisec;

		if (timers[i].Latency < 0)
		{
			timers[i].trackFunction();
			timers.erase(timers.begin() + i);
			--i;
		}
	}
}


void setTimer(function<void()> func, int millisec)
{
	timers.push_back(Timer(func, millisec));
}


struct Animation
{
	double* target;
	double start;
	double end;
	double distance;
	double duration;
	double totalTime;
	double curve = 0;

	Animation(double* target, double start, double end, int duration);
};

vector<Animation> animations;

Animation::Animation(double* target, double start, double end, int millisec) : target(target), start(start), duration(millisec), totalTime(millisec), end(end)
{ 
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].target == target)
		{
			animations.erase(animations.begin() + i);
			break;
		}
	}
	distance = end - start; 
}

bool deleteAnimation(double* target)
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animations[i].target == target)
		{
			animations.erase(animations.begin() + i);
			return true;
		}
	}

	return false;
}


void animationUpdate()
{
	int deltaMillisec = static_cast<int>(doodle::DeltaTime * 1000);

	for (int i = 0; i < animations.size(); ++i)
	{
		animations[i].duration -= deltaMillisec;

		if (animations[i].duration <= 0)
		{
			*(animations[i].target) = animations[i].end;
			animations.erase(animations.begin() + i);
			--i;
			continue;
		}

		*(animations[i].target) = animations[i].start + (1.0 - (animations[i].duration / animations[i].totalTime)) * animations[i].distance;
	}
}

void setAnimation(double* target, double start, double end, int milisec)
{
	animations.push_back(Animation(target, start, end, milisec));
}

