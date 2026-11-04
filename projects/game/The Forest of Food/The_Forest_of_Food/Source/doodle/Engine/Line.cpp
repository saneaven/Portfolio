/*
File name : Line.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Line.h"
#include <limits>


bool isPointInLineRange(const Line& line, const Vector2D<>& point)
{
	constexpr double epsilon{ std::numeric_limits<double>::epsilon() };
	if (
		((line.pointA.x - epsilon <= point.x && point.x <= line.pointB.x + epsilon) || (line.pointA.x >= point.x + epsilon && point.x >= line.pointB.x - epsilon)) &&
		((line.pointA.y - epsilon <= point.y && point.y <= line.pointB.y + epsilon) || (line.pointA.y >= point.y + epsilon && point.y >= line.pointB.y - epsilon))
		)
		return true;
	else
		return false;
}

[[nodiscard]] bool isCrossing(const Line& lineA, const Line& lineB)
{
	Vector2D crossingPoint;
	double det = (lineA.pointB - lineA.pointA).crossProduct(lineB.pointB - lineB.pointA);

	if (abs(det) < std::numeric_limits<double>::epsilon()) return false;

	crossingPoint = lineA.pointA + (lineA.pointB - lineA.pointA) * ((lineB.pointA - lineA.pointA).crossProduct(lineB.pointB - lineB.pointA) / det);

	if (isPointInLineRange(lineA, crossingPoint) && isPointInLineRange(lineB, crossingPoint))
	{
		std::cout << "point" << crossingPoint << " is in range of " << lineA.pointA << " and " << lineA.pointB << std::endl;
		std::cout << "point" << crossingPoint << " is in range of " << lineB.pointA << " and " << lineB.pointB << std::endl;
		return true;
	}
	else
	{
		//crossingPoint = Vector2D(false);
		return false;
	}
}