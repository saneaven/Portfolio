/*
File name : Rectangle.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Engine.h"


class TransformMatrix;

enum class Direction;

class Rectangle
{
	Vector2D<> leftDown; //rectangle vertex. (0,0) is hotspot
	Vector2D<> rightUp; //rectangle vertex. (0,0) is hotspot
	Vector2D<> hotSpot;

	int width;
	int height;

public:
	Rectangle(std::pair< Vector2D<>, Vector2D<> > pair, Vector2D<int> hotSpot)
		: leftDown(pair.first), rightUp(pair.second), hotSpot(hotSpot)
	{  
		width = static_cast<int>(rightUp.x) - static_cast<int>(leftDown.x);
		height = static_cast<int>(rightUp.y) - static_cast<int>(leftDown.y);
	}

	[[nodiscard]] constexpr const Vector2D<> GetPositionOf(Direction direction) const;
	[[nodiscard]] bool isPointInside(Vector2D<> point) const;
	[[nodiscard]] constexpr int GetWidth() const { return width; }
	[[nodiscard]] constexpr int GetHeight() const { return height; }

	[[nodiscard]] bool isOverlapped(Rectangle other) const;
	void Move(Vector2D<> delta);
	void SetPosition(Vector2D<> newspot);
	void Draw(TransformMatrix displayMatrix) const;
	void Draw() const;
};