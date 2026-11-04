/*
File name : Class_Bullet.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/
#include <iostream>
#include <doodle/doodle.hpp>
#include <vector>
#include <random>


#include "classes.h"


extern Player player;

bool Bullet::update(std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items, Gate gate)
{
	bool isHitted = false;

	this->gravity();
	this->setForceAccelation();
	this->accelerate();
	
	isHitted = this->collision(enemies, blocks, items, gate);
	this->move();


	this->resetForce();

	return isHitted;
}

bool Bullet::collision(std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items, Gate gate)
{
	bool isHitted = false;
	Direction collisionDirection = Direction::notD;
	Vec2 forcedMoveVector;
	for (Block& i : blocks)
	{
		if (this->collisionCheck(i, collisionDirection, forcedMoveVector))
		{
			this->collisionInteract(collisionDirection);

			collisionDirection = Direction::notD;
			isHitted = true;
		}
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		if (this->collisionCheck(enemies[i], collisionDirection, forcedMoveVector))
		{

			if (enemies[i].getColor() != 0xFF0000FF)
			{
				int random = rand() % 8;
				items.push_back(Item(enemies[i].getPosition(), random));

				enemies[i].play_note();
				enemies.erase(enemies.begin() + i);
				player.gateNotes[(int)enemies[i].getNote()]++;
				--i;
				continue;
			}

			Vec2 bulletForce;
			bulletForce.setLength(BULLETPOWER*10 / doodle::DeltaTime);
			bulletForce.setAngle(this->getVelocity().getAngle());
			enemies[i].setForce(bulletForce);
			collisionDirection = Direction::notD;

			enemies[i].play_note();

			isHitted = true;
		}
	}

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

Bullet::Bullet(Vec2 p, Vec2 v, Vec2 f) : Object(p, BULLETSIZE, BULLETSIZE, 1) { setVelocity(v); gainForce(f); isGravity = false; }
