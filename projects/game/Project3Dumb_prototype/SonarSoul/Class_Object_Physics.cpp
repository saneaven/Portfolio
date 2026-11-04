/*
File name : Class_Object_Physics.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <doodle/doodle.hpp>
#include "classes.h"
#include "basicFunc.h"


bool isPointInLineRange(const Vec2& a1, const Vec2& a2, const Vec2& p)
{
	if (((a1.xValue() <= p.xValue() && p.xValue() <= a2.xValue()) || (a1.xValue() >= p.xValue() && p.xValue() >= a2.xValue())) &&
		((a1.yValue() <= p.yValue() && p.yValue() <= a2.yValue()) || (a1.yValue() >= p.yValue() && p.yValue() >= a2.yValue())))
		return true;
	else
		return false;
}

bool isParallelIntersect(const Vec2& a1, const Vec2& a2,
	const Vec2& b1, const Vec2& b2)
{
	if (((a2 - a1).getAngle() - (b2 - b1).getAngle()) <= 0.1 || ((a2 - a1).getAngle() - (b2 - b1).getAngle() + PI) <= 0.1)
		if (isPointInLineRange(a1, a2, b1) || isPointInLineRange(a1, a2, b2))
			return true;
	return false;
}

bool GetIntersectPoint(const Vec2& a1, const Vec2& a2,
	const Vec2& b1, const Vec2& b2, Vec2& outP)
{
	double det = (a2 - a1).crossProduct(b2 - b1);

	if (abs(det) < FLT_EPSILON) return false;

	outP = a1 + (a2 - a1) * ((b1 - a1).crossProduct(b2 - b1) / det);

	if (isPointInLineRange(a1, a2, outP) && isPointInLineRange(b1, b2, outP))
		return true;
	else
	{
		outP = Vec2(0, 0);
		return false;
	}
}


void Object::friction()
{
	Vec2 frictionForce;

	if (maxSpeedX > 0)//x limit
	{
		if(this->velocity.xValue() >= 0)
			frictionForce = Vec2((STEPFORCE) * (sqrt(abs(this->velocity.xValue()) / maxSpeedX)), 0);
		else
			frictionForce = Vec2(-(STEPFORCE) * (sqrt(abs(this->velocity.xValue()) / maxSpeedX)), 0);;
	}


	this->force -= frictionForce;


	//if (maxSpeedY > 0)//y limit
	//{
	//	if (this->velocity.yValue() >= 0)
	//		frictionForce = Vec2(0, 0);
	//		//frictionForce = Vec2(0, G * (sqrt(abs(this->velocity.yValue()) / maxSpeedY)));
	//	else
	//		frictionForce = Vec2(0, -G * (sqrt(abs(this->velocity.yValue()) / maxSpeedY)));
	//}
	//this->force -= frictionForce;
}


bool Object::collisionCheck(const Object& check, Direction& conllisionDirection, Vec2& forcedMoveVector)
{
	conllisionDirection = Direction::notD;

	const double& thisX = this->position.xValue();
	const double& thisY = this->position.yValue();
	Vec2 thisEdge;
	Vec2 thisRelativeVelocity = this->getVelocity() - check.getVelocity();

	Vec2 MoveVector;

	const double& checkX = check.getPosition().xValue();
	const double& checkY = check.getPosition().yValue();
	const Vec2 checkA = check.getPositionOf(Direction::upLeft);
	const Vec2 checkB = check.getPositionOf(Direction::upRight);
	const Vec2 checkC = check.getPositionOf(Direction::downRight);
	const Vec2 checkD = check.getPositionOf(Direction::downLeft);

	if (!(this->getPositionOf(Direction::right).xValue() + this->getVelocity().xValue() >= check.getPositionOf(Direction::left).xValue()
		&& this->getPositionOf(Direction::left).xValue() + this->getVelocity().xValue() <= check.getPositionOf(Direction::right).xValue()
		&& this->getPositionOf(Direction::up).yValue() + this->getVelocity().yValue() >= check.getPositionOf(Direction::down).yValue()
		&& this->getPositionOf(Direction::down).yValue() + this->getVelocity().yValue() <= check.getPositionOf(Direction::up).yValue()))
	{
		return false;
	}


	if (thisX < checkX)
	{
		if (thisY < checkY)
			thisEdge = this->getPositionOf(Direction::upRight);
		else
			thisEdge = this->getPositionOf(Direction::downRight);
	}
	else
	{
		if (thisY < checkY)
			thisEdge = this->getPositionOf(Direction::upLeft);
		else
			thisEdge = this->getPositionOf(Direction::downLeft);
	}


	if (GetIntersectPoint(thisEdge, thisEdge + thisRelativeVelocity, checkA, checkB, MoveVector))
			conllisionDirection = Direction::up;//up
	else if (GetIntersectPoint(thisEdge, thisEdge + thisRelativeVelocity, checkB, checkC, MoveVector))
			conllisionDirection = Direction::right;//right
	else if (GetIntersectPoint(thisEdge, thisEdge + thisRelativeVelocity, checkC, checkD, MoveVector))
			conllisionDirection = Direction::down;//down
	else if (GetIntersectPoint(thisEdge, thisEdge + thisRelativeVelocity, checkD, checkA, MoveVector))
			conllisionDirection = Direction::left;//left

	if (MoveVector.isSimilarWith(checkA) || MoveVector.isSimilarWith(checkB) || MoveVector.isSimilarWith(checkC) || MoveVector.isSimilarWith(checkD))
	{
		conllisionDirection = Direction::edge;//edge
	}



	if(conllisionDirection != Direction::edge)
	{
		if (conllisionDirection != Direction::notD)
		{
			MoveVector -= thisEdge;

			forcedMoveVector += MoveVector;

			return true;
		}

		if (isParallelIntersect(thisEdge, thisEdge - thisRelativeVelocity, checkA, checkB))
			conllisionDirection = Direction::up;
		if (isParallelIntersect(thisEdge, thisEdge - thisRelativeVelocity, checkB, checkC))
			conllisionDirection = Direction::right;
		if (isParallelIntersect(thisEdge, thisEdge - thisRelativeVelocity, checkC, checkD))
			conllisionDirection = Direction::down;
		if (isParallelIntersect(thisEdge, thisEdge - thisRelativeVelocity, checkD, checkA))
			conllisionDirection = Direction::left;


		if (conllisionDirection == Direction::notD && check.isPointInObject(thisEdge))//some reason algorithm cannot found collision
		{

			Vec2 forcedMover;

			if (checkA.x < thisX && thisX < checkB.x)
			{
				if (thisY > checkY)//up
				{
					forcedMover = Vec2(0, check.getPositionOf(Direction::up).yValue() - thisEdge.yValue());
					conllisionDirection = Direction::up;
				}

				else if (thisY < checkY)//down
				{
					forcedMover = Vec2(0, check.getPositionOf(Direction::down).yValue() - thisEdge.yValue());
					conllisionDirection = Direction::down;
				}
			}
			else if (checkD.y < thisY && thisY < checkA.y)
			{
				if (thisX > checkX)//right
				{
					forcedMover = Vec2(check.getPositionOf(Direction::right).xValue() - thisEdge.xValue(), 0);
					conllisionDirection = Direction::right;
				}
				else if (thisX < checkX)//left
				{
					forcedMover = Vec2(check.getPositionOf(Direction::left).xValue() - thisEdge.xValue(), 0);
					conllisionDirection = Direction::left;
				}

			}



			this->forcedMove(forcedMover);
		}


		return true;
	}



	return false;
}

void Object::collisionInteract(Direction direction)
{
	switch (direction)
	{
	case Direction::up:
		if(this->getVelocity().yValue() < EPSILON)
			this->velocity = Vec2(this->velocity.xValue(), 0);
		break;
	case Direction::down:
		if (this->getVelocity().yValue() > -EPSILON)
			this->velocity = Vec2(this->velocity.xValue(), 0);
		break;
	case Direction::left:
		if (this->getVelocity().xValue() > -EPSILON)
			this->velocity = Vec2(0, this->velocity.yValue());
		break;
	case Direction::right:
		if (this->getVelocity().xValue() < EPSILON)
			this->velocity = Vec2(0, this->velocity.yValue());
		break;
	case Direction::edge:
		this->velocity = Vec2(0,0);
		break;
	}
}