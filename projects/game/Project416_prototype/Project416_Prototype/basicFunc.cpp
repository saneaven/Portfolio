/*
	File name: basicFunc.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	Author: Minjae Kyung
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <doodle/doodle.hpp>
#include "basicFunc.h"
#include "classes.h"

bool isPointInLineRange(const Vec2& a1, const Vec2& a2, const Vec2& p)
{
	if (((a1.xValue() <= p.xValue() && p.xValue() <= a2.xValue()) || (a1.xValue() >= p.xValue() && p.xValue() >= a2.xValue())) &&
		((a1.yValue() <= p.yValue() && p.yValue() <= a2.yValue()) || (a1.yValue() >= p.yValue() && p.yValue() >= a2.yValue())))
		return true;
	else
		return false;
}

bool GetIntersectPoint(const Vec2& a1, const Vec2& a2,
	const Vec2& b1, const Vec2& b2, Vec2& outP)
{
	if (a1.xValue() == a2.xValue() && b1.xValue() == b2.xValue())
	{
		if (a1.xValue() == b1.xValue())
		{
			if (isPointInLineRange(a1, a2, b1) || isPointInLineRange(a1, a2, b2))
			{
				outP = a1;
				return true;
			}
			else if (isPointInLineRange(b1, b2, a1) || isPointInLineRange(b1, b2, a2))
			{
				outP = a1;
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	else if (a1.xValue() == a2.xValue())
	{
		outP = Vec2(a1.xValue(), (b2 - b1).getSlope() * (a1.xValue() - b1.xValue()) + b1.yValue());

		if (isPointInLineRange(a1, a2, outP) && isPointInLineRange(b1, b2, outP))
			return true;
		else
			return false;
	}
	else if (b1.xValue() == b2.xValue())
	{
		outP = Vec2(b1.xValue(), (a2 - a1).getSlope() * (b1.xValue() - a2.xValue()) + a2.yValue());

		if (isPointInLineRange(a1, a2, outP) && isPointInLineRange(b1, b2, outP))
			return true;
		else
			return false;
	}
	else if (a1.yValue() == a2.yValue() && b1.yValue() == b2.yValue())
	{
		if (a1.yValue() == b1.yValue())
		{
			if (isPointInLineRange(a1, a2, b1) || isPointInLineRange(a1, a2, b2))
			{
				outP = a1;
				return true;
			}
			else if (isPointInLineRange(b1, b2, a1) || isPointInLineRange(b1, b2, a2))
			{
				outP = a1;
				return true;
			}
			else
				return false;
		}
		else
			return false;

	}
	else
	{
		double upper_t = (b2.xValue() - b1.xValue()) * (a1.yValue() - b1.yValue()) - (b2.yValue() - b1.yValue()) * (a1.xValue() - b1.xValue());
		double upper_s = (a2.xValue() - a1.xValue()) * (a1.yValue() - b1.yValue()) - (a2.yValue() - a1.yValue()) * (a1.xValue() - b1.xValue());
		double under = (b2.yValue() - b1.yValue()) * (a2.xValue() - a1.xValue()) - (b2.xValue() - b1.xValue()) * (a2.yValue() - a1.yValue());

		if (under == 0) return false;

		double t = upper_t / under;
		double s = upper_s / under;

		if (t < 0. || 1. < t || s < 0. || 1. < s) return false;
		if (upper_t == 0 && upper_s == 0) return false;

		outP = Vec2(a1.xValue() + t * (a2.xValue() - a1.xValue()), a1.yValue() + t * (a2.yValue() - a1.yValue()));
	}

	if (isPointInLineRange(a1, a2, outP) && isPointInLineRange(b1, b2, outP))
		return true;
	else
		return false;
}