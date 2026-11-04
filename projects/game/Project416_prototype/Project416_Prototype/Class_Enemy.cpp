/*
	File name: Class_Enemy.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	primary author: Minjae Kyung
	secondary author: Haeun Park
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <iostream>
#include <doodle/doodle.hpp>
#include <cmath>
#include <vector>

#include "classes.h"


Enemy::Enemy(Vec2 pos, int t) : Object(pos), type(t)
{
	initEnemyByType(t);
}
Enemy::Enemy(double x, double y, int t) : Object(x, y), type(t) 
{
	initEnemyByType(t);
}

void Enemy::initEnemyByType(int t)
{
	switch (t)
	{
	case 0: // RED
		setWidth(40);
		setHeight(45);
		setColor(0xFF0000FF);
		break;
	case 1: // GREEN
		setWidth(40);
		setHeight(45);
		setColor(0x00FF00FF);
		break;
	case 2: // BLUE
		setWidth(40);
		setHeight(45);
		setColor(0x0000FFFF);
		break;
	case 3: // BLACK
		setWidth(40);
		setHeight(45);
		setColor(0x000000FF);
		break;
	}
}

void Enemy::walk()
{
	//walkForce = constWalkForce * doodle::DeltaTime;
	Vec2 constWalkForce = Vec2{ 150,0 };
	if (this->walkDirection == Direction::left)
		walkForce = Vec2(constWalkForce.xValue() * -1, constWalkForce.yValue());
	else
		walkForce = constWalkForce;

	this->gainForce(walkForce);
}



void Enemy::detectBlock(const std::vector<Block>& blocks)
{
	bool p1BlockOverlap = false;
	bool p2BlockOverlap = false;

	Vec2 p1(this->getPositionOf(Direction::downLeft).xValue() - 1., this->getPositionOf(Direction::downLeft).yValue() - 10.);
	Vec2 p2(this->getPositionOf(Direction::downRight).xValue() + 1., this->getPositionOf(Direction::downRight).yValue() - 10.);

	for (const Block& i : blocks)
	{
		if (i.isPointInObject(p1))
			p1BlockOverlap = true;
		if (i.isPointInObject(p2))
			p2BlockOverlap = true;
	}



	if (!p1BlockOverlap && this->walkDirection == Direction::left)
	{
		this->walkDirection = Direction::right;
		this->setVelocity(Vec2(0, this->getLawVelocity().yValue()));
	}
	else if (!p2BlockOverlap && this->walkDirection == Direction::right)
	{
		this->walkDirection = Direction::left;
		this->setVelocity(Vec2(0, this->getLawVelocity().yValue()));
	}
}

void Enemy::update(const std::vector<Block>& blocks)
{
	Direction collisionDirection = Direction::notD;


	this->walk();
	this->gravity();
	this->friction();
	this->setForceAccelation();
	this->accelerate();
	this->WallCollisionCheck();
	this->detectBlock(blocks);
	this->move();


	for (Block i : blocks)
	{
		if (this->getColor() != i.getColor())
		{
			collisionDirection = this->collisionCheck(i);

			if (collisionDirection != Direction::notD)
			{
				this->collisionInteract(collisionDirection);

				switch (collisionDirection)
				{
				case Direction::up:
					this->isHittedUp = true;
					break;
				case Direction::down:
					this->isHittedDown = true;
					break;
				case Direction::left:
					this->walkDirection = Direction::left;
					this->isHittedLeft = true;
					break;
				case Direction::right:
					this->walkDirection = Direction::right;
					this->isHittedRight = true;
					break;
				}

				collisionDirection = Direction::notD;
			}
		}
	}

	this->resetForce();
}