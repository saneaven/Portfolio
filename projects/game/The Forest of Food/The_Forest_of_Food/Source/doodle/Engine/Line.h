/*
File name : Line.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Vector2D.h"


struct Line
{
	Vector2D<> pointA;
	Vector2D<> pointB;

	Line(Vector2D<> A, Vector2D<> B) : pointA(A), pointB(B) {}
};

bool isPointInLineRange(const Line& line, const Vector2D<>& point);

[[nodiscard]] bool isCrossing(const Line& lineA, const Line& lineB);