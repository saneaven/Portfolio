/*
File name : Rectangle.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Rectangle.h"

#include <doodle/doodle.hpp>

[[nodiscard]] constexpr const Vector2D<> Rectangle::GetPositionOf(Direction direction) const
{
	switch (direction)
	{
	case Direction::center:
		return { (leftDown.x + rightUp.x) / 2, (leftDown.y + rightUp.y) / 2 };
	case Direction::up:
		return { (leftDown.x + rightUp.x) / 2, rightUp.y };
	case Direction::down:
		return { (leftDown.x + rightUp.x) / 2, leftDown.y };
	case Direction::left:
		return { leftDown.x, (leftDown.y + rightUp.y) / 2 };
	case Direction::right:
		return { rightUp.x, (leftDown.y + rightUp.y) / 2 };
	case Direction::leftUp:
		return { leftDown.x, rightUp.y };
	case Direction::rightUp:
		return rightUp;
	case Direction::leftDown:
		return leftDown;
	case Direction::rightDown:
		return { rightUp.x, leftDown.y };
	}

	return { -1,-1 };
}

[[nodiscard]] bool Rectangle::isPointInside(Vector2D<> point) const
{
	if (GetPositionOf(Direction::left).x <= point.x && point.x <= GetPositionOf(Direction::right).x &&
		GetPositionOf(Direction::down).y <= point.y && point.y <= GetPositionOf(Direction::up).y)
	{
		return true;
	}
	else
		return false;
}

[[nodiscard]] bool Rectangle::isOverlapped(Rectangle other) const
{
	if (
		abs(GetPositionOf(Direction::center).x - other.GetPositionOf(Direction::center).x) < (GetWidth() + other.GetWidth()) / 2 &&
		abs(GetPositionOf(Direction::center).y - other.GetPositionOf(Direction::center).y) < (GetHeight() + other.GetHeight()) / 2
		)
	{
		return true;
	}
	else
		return false;
}

void Rectangle::Move(Vector2D<> delta)
{
	leftDown += delta;
	rightUp += delta;
	hotSpot += delta;
}

void Rectangle::SetPosition(Vector2D<> newspot)
{
	leftDown += newspot - hotSpot;
	rightUp += newspot - hotSpot;
	hotSpot = newspot;
}

void Rectangle::Draw() const
{
	doodle::push_settings();
	doodle::set_fill_color(0x00aa00ff);
	doodle::draw_rectangle(GetPositionOf(Direction::center).x, GetPositionOf(Direction::center).y, width, height);
	doodle::pop_settings();
}

void Rectangle::Draw(TransformMatrix displayMatrix) const
{
	doodle::push_settings();
	doodle::no_fill();
	doodle::set_rectangle_mode(doodle::RectMode::Center);
	doodle::apply_matrix
	(
		displayMatrix[0][0], displayMatrix[1][0],
		displayMatrix[0][1], displayMatrix[1][1],
		displayMatrix[0][2], displayMatrix[1][2]
	);
	doodle::draw_rectangle(0, 0, width, height);
	doodle::pop_settings();
}