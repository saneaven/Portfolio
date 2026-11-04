/*
File name : BasicDataTypes.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "BasicDataTypes.h"

Direction reverseDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::up:
		return Direction::down;
	case Direction::down:
		return Direction::up;
	case Direction::left:
		return Direction::right;
	case Direction::right:
		return Direction::left;
	case Direction::leftUp:
		return Direction::rightDown;
	case Direction::rightUp:
		return Direction::leftDown;
	case Direction::rightDown:
		return Direction::leftUp;
	case Direction::leftDown:
		return Direction::rightUp;
	}

	return direction;
}