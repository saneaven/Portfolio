/*
File name : Class_Enemy.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Haneul Lee
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#include <iostream>
#include <doodle/doodle.hpp>
#include <cmath>
#include <vector>

#include "classes.h"
#include "SFX.h"


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
	this->setMass(1);
	switch (t)
	{
	case 0:
		this->monster_note = Syllable::Do;
		break;
	case 1:
		this->monster_note = Syllable::Re;
		break;
	case 2:
		this->monster_note = Syllable::Mi;
		break;
	case 3:
		this->monster_note = Syllable::Fa;
		break;
	case 4:
		this->monster_note = Syllable::Sol;
		break;
	case 5:
		this->monster_note = Syllable::La;
		break;
	case 6:
		this->monster_note = Syllable::Ti;
		break;
	case 7:
		this->monster_note = Syllable::Do_high;
		break;
	default:
		break;
	}
}

Syllable Enemy::getNote() { return this->monster_note; }

void Enemy::play_note()
{
	for (auto& sound : E_sounds)
	{
		if (sound.getStatus() != sf::SoundSource::Playing)
		{
			sound.setBuffer(E_soundBuffers[(int)this->monster_note]);
			sound.play();
			std::cout << "sound test\n";
		}
	}


	E_sounds.emplace_back(E_soundBuffers[(int)this->monster_note]);
	E_sounds.back().play();
	return;
}

void Enemy::walk()
{
	Vec2 constWalkForce = Vec2{ STEPFORCE * 1.5 ,0 };//need to change per enemy type;
	if (this->walkDirection == Direction::left)
		walkForce = Vec2(constWalkForce.xValue() * -1, constWalkForce.yValue());
	else
		walkForce = constWalkForce;

	this->gainForce(walkForce);
}


void Enemy::update(const std::vector<Block>& blocks)
{
	Direction collisionDirection = Direction::notD;

	this->walk();
	this->gravity();
	this->friction();
	this->setForceAccelation();
	this->accelerate();

	bool left_point_overlap = false;
	bool right_point_overlap = false;
	Vec2 forcedMoveVector;

	for (Block i : blocks)
	{
		Vec2 left_point(this->getPositionOf(Direction::downLeft).xValue() - 1, this->getPositionOf(Direction::downLeft).yValue() - 5);
		Vec2 right_point(this->getPositionOf(Direction::downRight).xValue() + 1, this->getPositionOf(Direction::downRight).yValue() - 5);

		if (i.isPointInObject(left_point))
		{
			left_point_overlap = true;
		}
		if (i.isPointInObject(right_point))
		{
			right_point_overlap = true;
		}

		if (this->collisionCheck(i, collisionDirection, forcedMoveVector))
		{
			this->collisionInteract(collisionDirection);

			switch (collisionDirection)
			{
			case Direction::up:
				break;
			case Direction::down:
				break;
			case Direction::left:
				this->stop();
				this->walkDirection = Direction::left;
				break;
			case Direction::right:
				this->stop();
				this->walkDirection = Direction::right;
				break;
			}

			collisionDirection = Direction::notD;
		}
	}

	if (!left_point_overlap && this->walkDirection == Direction::left)
	{
		this->stop();
		this->walkDirection = Direction::right;
	}
	else if (!right_point_overlap && this->walkDirection == Direction::right)
	{
		this->stop();
		this->walkDirection = Direction::left;
	}
	this->forcedMove(forcedMoveVector);

	this->move();

	this->resetForce();
}