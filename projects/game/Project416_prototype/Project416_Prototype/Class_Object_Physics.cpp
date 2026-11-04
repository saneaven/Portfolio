/*
	File name: Class_Object_Physics.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	Author: Minjae Kyung
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <doodle/doodle.hpp>
#include "classes.h"
#include "basicFunc.h"

void Object::friction()
{
	Vec2 frictionForce;

	if (maxSpeedX > 0)//x limit
	{
		if(this->velocity.xValue() >= 0)
			frictionForce = Vec2(STEPFORCE * (sqrt(abs(this->velocity.xValue()) / maxSpeedX)), 0);
		else
			frictionForce = Vec2(-STEPFORCE * (sqrt(abs(this->velocity.xValue()) / maxSpeedX)), 0);
	}


	this->force -= frictionForce;


	if (maxSpeedY > 0)//y limit
	{
		if (this->velocity.yValue() >= 0)
			frictionForce = Vec2(0, 0);
			//frictionForce = Vec2(0, G * (sqrt(abs(this->velocity.yValue()) / maxSpeedY)));
		else
			frictionForce = Vec2(0, -G * (sqrt(abs(this->velocity.yValue()) / maxSpeedY)));
	}


	this->force -= frictionForce;
}


Direction Object::collisionCheck(Object& check)
{
	Direction conllisionDirection = Direction::notD;
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


	if (!(this->getPositionOf(Direction::right).xValue() >= check.getPositionOf(Direction::left).xValue()
		&& this->getPositionOf(Direction::left).xValue() <= check.getPositionOf(Direction::right).xValue()
		&& this->getPositionOf(Direction::up).yValue() >= check.getPositionOf(Direction::down).yValue()
		&& this->getPositionOf(Direction::down).yValue() <= check.getPositionOf(Direction::up).yValue()))
	{
		return conllisionDirection;
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


	if (GetIntersectPoint(thisEdge, thisEdge - thisRelativeVelocity, checkA, checkB, MoveVector))
	{
		if (MoveVector == checkA || MoveVector == checkB)
			conllisionDirection = Direction::edge;//edge
		else
			conllisionDirection = Direction::up;//up
	}
	else if (GetIntersectPoint(thisEdge, thisEdge - thisRelativeVelocity, checkB, checkC, MoveVector))
	{
		if (MoveVector == checkB || MoveVector == checkC)
			conllisionDirection = Direction::edge;
		else
			conllisionDirection = Direction::right;//right
	}
	else if (GetIntersectPoint(thisEdge, thisEdge - thisRelativeVelocity, checkC, checkD, MoveVector))
	{
		if (MoveVector == checkC || MoveVector == checkD)
			conllisionDirection = Direction::edge;
		else
			conllisionDirection = Direction::down;//down
	}
	else if (GetIntersectPoint(thisEdge, thisEdge - thisRelativeVelocity, checkD, checkA, MoveVector))
	{
		if (MoveVector == checkD || MoveVector == checkA)
			conllisionDirection = Direction::edge;
		else
			conllisionDirection = Direction::left;//left
	}


	

	if (conllisionDirection != Direction::notD && conllisionDirection != Direction::edge)
	{
		MoveVector -= thisEdge;
		this->forcedMove(MoveVector);
	}

	return conllisionDirection;
}

void Object::collisionInteract(Direction direction)
{
	switch (direction)
	{
	case Direction::up:
		if(this->getVelocity().yValue() < 0)
			this->velocity = Vec2(this->velocity.xValue(), 0);
		break;
	case Direction::down:
		if (this->getVelocity().yValue() > 0)
			this->velocity = Vec2(this->velocity.xValue(), 0);
		break;
	case Direction::left:
		if (this->getVelocity().xValue() > 0)
			this->velocity = Vec2(0, this->velocity.yValue());
		break;
	case Direction::right:
		if (this->getVelocity().xValue() < 0)
			this->velocity = Vec2(0, this->velocity.yValue());
		break;
	case Direction::edge:
		break;
	}
}

void Object::WallCollisionCheck()
{
	if (this->isHittedUp && this->getVelocity().yValue() < 0)
		this->velocity = Vec2(this->velocity.xValue(), 0);
	if (this->isHittedDown && this->getVelocity().yValue() > 0)
		this->velocity = Vec2(this->velocity.xValue(), 0);
	if (this->isHittedRight && this->getVelocity().xValue() < 0)
		this->velocity = Vec2(0, this->velocity.yValue());
	if (this->isHittedLeft && this->getVelocity().xValue() > 0)
		this->velocity = Vec2(0, this->velocity.yValue());

	this->isHittedRight = false, this->isHittedLeft = false, this->isHittedUp = false, this->isHittedDown = false;

}