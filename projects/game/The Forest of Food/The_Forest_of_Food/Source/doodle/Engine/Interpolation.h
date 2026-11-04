/*
File name : Interpolation.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <list>
#include <vector>
#include "Vector2D.h"

class interpolationManager;

struct BezierInterpolation
{
	Vector2D<>* track;

protected:
	Vector2D<> P1;// (start,0)
	Vector2D<> P2;// (start, startBezier) 
	Vector2D<> P3;// (end,0)
	Vector2D<> P4;// (end, endBezier) 

	double durationTime; // Interplortation duration time;
	double nowTime; // now progression time

	bool isDead;

public:
	BezierInterpolation();
	BezierInterpolation(Vector2D<>* track, Vector2D<> P1, Vector2D<> P2, Vector2D<> P3, Vector2D<> P4, double duration);

	Vector2D<> GetNowPosition(double progress);
	double GetRemainTime() { return durationTime - nowTime; }
	virtual void Update(double dt);
	bool IsDead(void) { return isDead; }

	friend class interpolationManager;
};

class TimeInterpolation : public BezierInterpolation
{
	double* track;

	double lastT;

	double FindPosition(double time);

public:
	TimeInterpolation(double* track, double start, double end, double duration, double startBezierStrength = 0, double endBezierStrength = 2);
	//beziers must between -1 ~ 1 (if endBezier == 2, same as startBezier)
	//if bezier strength is out of range, it will do undefined thing

	void Update(double dt) override;
};

class InterpolationManager
{
	std::list<BezierInterpolation*> interpolations;
	std::vector<BezierInterpolation*> destroyList;

public:
	void AddTimeLerp(double* track, double start, double end, double duration, double startBezierStrength = 0, double endBezierStrength = 2);
	void AddSpaceLerp(Vector2D<>* track, Vector2D<> P1, Vector2D<> P2, Vector2D<> P3, Vector2D<> P4, double duration);
	void Update(double dt);
};