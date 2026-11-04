/*
	File name: Class_Player.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	primary author: Minjae Kyung
	secondary author: Haeun Park
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <vector>
#include <iostream>
#include <doodle/doodle.hpp>

#include "classes.h"

void Player::keyCheck(std::vector<Bullet>& Bullets)
{
	Vec2 sigmaF;

	if (this->isRight)
		sigmaF += Vec2(STEPFORCE, 0);
	if (this->isLeft)
		sigmaF -= Vec2(STEPFORCE, 0);
	if (this->isJump)
	{
		sigmaF += Vec2(0, JUMPFORCE/doodle::DeltaTime);
		this->isJump = false;
	}
	if (this->isShoot)
	{
		if (this->ballStorage > 0)
		{
			Vec2 muzzle(5, 0);
			Bullets.push_back(Bullet(this->getPositionOf(this->staringDirection) - muzzle, Vec2(this->getLawVelocity().xValue(), 0), this->staringDirection, this->ball_color));
			this->ballStorage--;
		}
		this->isShoot = false;
	}


	this->gainForce(sigmaF);
}

void Player::setBallColor(uint32_t col) { this->ball_color = col; }

uint32_t Player::getBallColor() const { return ball_color; }
int Player::getBallStorage() const { return ballStorage; }


void Player::update(std::vector<Bullet>& Bullets, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items)
{
	Direction collisionDirection = Direction::notD;

	if (this->isRunning)
		this->maxSpeedX = MAXSPEED * 2, this->maxSpeedY = MAXSPEED * 4;
	else
		this->maxSpeedX = MAXSPEED, this->maxSpeedY = MAXSPEED * 3;


	this->keyCheck(Bullets);
	this->gravity();
	this->friction();
	this->setForceAccelation();
	this->accelerate();
	this->WallCollisionCheck();
	this->move();


	this->isOnGround = false;
	for (Block i : blocks)
	{
		if (this->getColor() != i.getColor())
		{
			collisionDirection = this->collisionCheck(i);

			if (collisionDirection != Direction::notD)
			{

				switch (collisionDirection)
				{
				case Direction::up:
					this->isHittedUp = true;
					this->isOnGround = true;
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

				this->collisionInteract(collisionDirection);
				collisionDirection = Direction::notD;
			}
		}
	}



	for (int i = 0; i < enemies.size(); i++)
	{
		collisionDirection = this->collisionCheck(enemies[i]);

		if (collisionDirection != Direction::notD)
		{
			collisionDirection = this->collisionCheck(enemies[i]);

			switch (collisionDirection)
			{
			case Direction::up:
				if (this->getColor() == enemies[i].getColor())
				{
					this->isJump = true;
					enemies.erase(enemies.begin() + i);
					--i;
				}
				else { this->isDead = true; }
				break;
			case Direction::down:
				this->isDead = true;
				break;
			case Direction::left:
				this->isDead = true;
				break;
			case Direction::right:
				this->isDead = true;
				break;
			}

			this->collisionInteract(collisionDirection);
			collisionDirection = Direction::notD;
		}
	}

	for (int i = 0; i < items.size(); i++)
	{
		
		if (this->getPositionOf(Direction::right).xValue() >= items[i].getPositionOf(Direction::left).xValue()
			&& this->getPositionOf(Direction::left).xValue() <= items[i].getPositionOf(Direction::right).xValue()
			&& this->getPositionOf(Direction::up).yValue() >= items[i].getPositionOf(Direction::down).yValue()
			&& this->getPositionOf(Direction::down).yValue() <= items[i].getPositionOf(Direction::up).yValue())
		{
			collisionDirection = Direction::center;
		}

		if (collisionDirection != Direction::notD)
		{
			collisionDirection = this->collisionCheck(items[i]);
			items[i].interact(*this);

			items.erase(items.begin() + i);
			--i;
			collisionDirection = Direction::notD;
		}
	}


	this->resetForce();
}

void Player::drawBall(Camera camera)
{
	using namespace doodle;

	const float& magnification = (float)camera.getMagnification();
	float thisX;
	const float thisY = (float)(this->getPosition().yValue() - camera.getPosition().yValue()) * magnification;

	if (this->staringDirection == Direction::right)
		thisX = (float)(this->getPositionOf(Direction::right).xValue() + BULLETSIZE/2 - camera.getPosition().xValue()) * magnification;
	else if (this->staringDirection == Direction::left)
		thisX = (float)(this->getPositionOf(Direction::left).xValue() - BULLETSIZE/2 - camera.getPosition().xValue()) * magnification;
	else
		thisX = 0;

	push_settings();
	set_fill_color(HexColor{ this->getBallColor() });
	draw_rectangle(thisX, thisY, (float)BULLETSIZE * magnification, (float)BULLETSIZE * magnification);
	pop_settings();
}