/*
File name : BasicDataTypes.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "Vector2D.h"

struct Rect
{

	Vector2D<>LeftBottom;
	Vector2D<>TopRight;

};


enum class Direction
{
	notD = -1, center, up, down, left, right, leftUp, rightUp, rightDown, leftDown
};
Direction reverseDirection(Direction direction);

std::ostream& operator<<(std::ostream& output, const Direction direction);



typedef unsigned Color;