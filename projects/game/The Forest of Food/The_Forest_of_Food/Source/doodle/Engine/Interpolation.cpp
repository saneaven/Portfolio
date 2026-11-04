/*
File name : Interpolation.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Interpolation.h"
#include <string>

BezierInterpolation::BezierInterpolation()
	:track(nullptr), P1(0), P2(0), P3(0), P4(0), durationTime(0), nowTime(0), isDead(false)
{

}

BezierInterpolation::BezierInterpolation(Vector2D<>* track, Vector2D<> P1, Vector2D<> P2, Vector2D<> P3, Vector2D<> P4, double duration)
	:track(track), P1(P1), P2(P2), P3(P3), P4(P4), durationTime(duration), nowTime(0), isDead(false)
{

}

Vector2D<> BezierInterpolation::GetNowPosition(double progress)
{
	return (P1 * pow(1 - progress, 3)) + (P2 * progress * pow(1 - progress, 2) * 3) + (P3 * pow(progress, 2) * (1 - progress) * 3) + (P4 * pow(progress, 3));
}

void BezierInterpolation::Update(double dt)
{
	nowTime += dt;
	double progress = nowTime / durationTime;

	if (progress <= 1)
	{
		*track = GetNowPosition(progress);
	}
	else
	{
		*track = P4;
		isDead = true;
	}
}



TimeInterpolation::TimeInterpolation(double* track, double start, double end, double duration, double startBezierStrength, double endBezierStrength)
	: track(track), lastT(0)
{
	durationTime = duration;
	nowTime = 0;

	isDead = false;


	if ((startBezierStrength < -1 || 1 < startBezierStrength)
		|| (endBezierStrength < -1 || 1 < endBezierStrength))
	{
		std::cout << "error : beziers must between -1 ~ 1";
	}
	else if ((startBezierStrength < 0)
		&& (endBezierStrength < 0))
	{
		std::cout << "error : one of beziers must bigger than 0";
	}

	if (endBezierStrength == 2)
		endBezierStrength = startBezierStrength;

	P1 = { 0, start };

	if (startBezierStrength > 0)
		P2 = { startBezierStrength, start };
	else
		P2 = { 0, end + startBezierStrength };

	if (endBezierStrength > 0)
		P3 = { 1 - endBezierStrength, end };
	else
		P3 = { 1, start - endBezierStrength };

	P4 = { 1, end };


}

void TimeInterpolation::Update(double dt)
{
	nowTime += dt;
	double progress = nowTime / durationTime;

	if (progress <= 1)
	{
		*track = FindPosition(progress);
	}
	else
	{
		*track = P4.y;
		isDead = true;
	}
}


#include "doodle/doodle.hpp"
constexpr double SEARCH_ACCURACY = 1000;

double TimeInterpolation::FindPosition(double timeProgress)
{
	Vector2D<> result = { 0,0 };
	double progress;

	for (progress = lastT; result.x < timeProgress; progress += (1 / SEARCH_ACCURACY))
	{
		result = GetNowPosition(progress);
		//x->y를 구해야 하는데 t->y를 구하고 있어서 임시로 노가다시킴 ㅅㅂ
		//doodle::draw_ellipse(result.x * 1000, result.y / 2 , 10);
	}

	lastT = progress;

	//doodle::draw_ellipse(P1.x * 1000, P1.y / 2, 40);
	//doodle::draw_ellipse(P2.x * 1000, P2.y / 2, 40);
	//doodle::draw_ellipse(P3.x * 1000, P3.y / 2, 40);
	//doodle::draw_ellipse(P4.x * 1000, P4.y / 2, 40);

	//doodle::draw_text(std::to_string(timeProgress), 1000, 0);
	//doodle::draw_text(std::to_string(*track), 0, 0);
	return result.y;
}

void InterpolationManager::AddTimeLerp(double* track, double start, double end, double duration, double startBezierStrength, double endBezierStrength)
{
	interpolations.push_back(new TimeInterpolation{ track, start, end, duration, startBezierStrength, endBezierStrength });
}

void InterpolationManager::AddSpaceLerp(Vector2D<>* track, Vector2D<> P1, Vector2D<> P2, Vector2D<> P3, Vector2D<> P4, double duration)
{
	interpolations.push_back(new BezierInterpolation{ track, P1, P2, P3, P4, duration });
}

void InterpolationManager::Update(double dt)
{
	for (BezierInterpolation* i : interpolations)
	{
		i->Update(dt);
		if (i->IsDead())
		{
			destroyList.push_back(i);
		}
	}

	for (BezierInterpolation* i : destroyList)
	{
		interpolations.remove(i);
		delete i;
	}
	destroyList.clear();
}