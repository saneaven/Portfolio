/*
	File name: Class_Bullet.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	Author: Minjae Kyung
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <iostream>
#include <doodle/doodle.hpp>
#include <vector>

#include "classes.h"

bool Bullet::update(std::vector<Enemy>& enemies, std::vector<Block>& blocks)
{
	Direction collisionDirection = Direction::notD;
	bool isHitted = false;

	this->gravity();
	this->setForceAccelation();
	this->accelerate();
	this->WallCollisionCheck();
	this->move();

	for (Block& i : blocks)
	{
		collisionDirection = this->collisionCheck(i);

		if (collisionDirection != Direction::notD)
		{
			this->collisionInteract(collisionDirection);


			i.hitbyball(this->getColor());

			collisionDirection = Direction::notD;
			isHitted = true;
		}
	}

	for (Enemy& i : enemies)
	{
		collisionDirection = this->collisionCheck(i);

		if (collisionDirection != Direction::notD)
		{
			this->collisionInteract(collisionDirection);

			i.hitbyball(this->getColor());

			switch (collisionDirection)
			{
			case Direction::up:
				this->isHittedUp = true;
				break;
			case Direction::down:
				this->isHittedDown = true;
				break;
			case Direction::left:
				this->isHittedLeft = true;
				break;
			case Direction::right:
				this->isHittedRight = true;
				break;
			}

			collisionDirection = Direction::notD;
			isHitted = true;
		}
	}

	this->resetForce();

	return isHitted;
}


Bullet::Bullet(Vec2 p, Vec2 v, Direction d, uint32_t c) : Object(p, BULLETSIZE, BULLETSIZE, 1)
{
	Vec2 BulletForce;

	setVelocity(v);

	BulletForce.setLength(BULLETPOWER / doodle::DeltaTime);
	if(d == Direction::right)
		BulletForce.setAngle(PI / 6);
	else if (d == Direction::left)
		BulletForce.setAngle(PI - PI / 6);
	gainForce(BulletForce);

	setColor(c);
}
Bullet::Bullet(double x, double y, uint32_t c) : Object(x, y, BULLETSIZE, BULLETSIZE, 1)
{
	Vec2 BulletForce;

	BulletForce.setLength(BULLETPOWER / doodle::DeltaTime);
	BulletForce.setAngle(PI / 6);
	gainForce(BulletForce);

	setColor(c);
}
Bullet::Bullet(double x, double y, Vec2 f, uint32_t c) : Object(x, y, BULLETSIZE, BULLETSIZE, 1) { gainForce(f); setColor(c); }
