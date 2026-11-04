/*
File name : Class_Player.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Haneul Lee
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#include <vector>
#include <iostream>
#include <doodle/doodle.hpp>

#include "classes.h"
#include "SFX.h"
#include "basicFunc.h"

void shoot_sound() 
{
	for (auto& sound : Shoot_sounds)
	{
		if (sound.getStatus() != sf::SoundSource::Playing)
		{
			sound.setBuffer(Shoot_Buffers[0]);
			sound.play();
		}
	}

	Shoot_sounds.emplace_back(Shoot_Buffers[0]);
	Shoot_sounds.back().play();
}

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
		//todo
		if (shootDelay == 0 && this->magazine > 0)
		{
			shootBullet(Bullets);
			--this->magazine;
		}

		this->isShoot = false;
	}


	this->gainForce(sigmaF);
}

void Player::shootBullet(std::vector<Bullet>& Bullets)
{
	this->resetPhysics();

	Vec2 bulletForce;
	
	bulletForce.setLength(BULLETPOWER/doodle::DeltaTime);
	bulletForce.setAngle(this->mouseAngle);

	Bullets.push_back(Bullet(this->getPosition(), this->getVelocity(), bulletForce));
	
	setAnimation(&(this->shootDelay), 0, SHOOTDELAY, (int)SHOOTDELAY);
	setTimer([this]() { this->shootDelay = 0; }, (int)SHOOTDELAY);

	this->gainForce(-bulletForce);

	shoot_sound();
}

void Player::reloadBullet()
{
	if (this->reloadTime == 0)
	{
		this->magazine = 0;
		setAnimation(&(this->reloadTime), 0, RELOADTIME, (int)RELOADTIME);
		setTimer([this]() { this->reloadTime = 0; this->magazine = 2; }, (int)RELOADTIME);
	}
}

void Player::use_note(bool& note_key, Syllable buffer_index)
{
	if (!note_key)
		return;

	for (auto& sound : P_sounds)
	{
		if (sound.getStatus() != sf::SoundSource::Playing)
		{
			sound.setBuffer(P_soundBuffers[(int)buffer_index]);
			sound.play();
		}
	}

	P_sounds.emplace_back(P_soundBuffers[(int)buffer_index]);
	P_sounds.back().play();

 	sonars.push_back(Sonar(this->getPosition(), 10, buffer_index));
	note_key = false;
}

void Player::use_gateSonar(std::vector<GateSonar>& gateSonars, Syllable syl)
{
	if (this->gateNotes[(int)syl] > 0)
	{
		gateSonars.push_back(GateSonar(this->getPosition(), 0, syl));
		--this->gateNotes[(int)syl];
	}
}

void Player::crouch(bool isCrouch)
{
	if (isCrouch)
		this->setHeight(40 / 2);
	else
		this->setHeight(40);
}

void Player::relativeMouseUpdate(const Camera& camera)
{
	Vec2 relativePosition = Vec2(doodle::get_mouse_x(), doodle::get_mouse_y()) - (this->getPosition() - camera.getPosition());

	this->mouseAngle = relativePosition.getAngle();
	this->mouseLength = relativePosition.getLength();
}

void Player::update(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items)
{
	if (this->isRunning)
		this->maxSpeedX = MAXSPEED * 2, this->maxSpeedY = MAXSPEED * 4;
	else
		this->maxSpeedX = MAXSPEED, this->maxSpeedY = MAXSPEED * 3;

	this->keyCheck(bullets);
	this->gravity();
	this->friction();
	this->setForceAccelation();
	this->accelerate();
	if(this->isGravity)
		this->collision(enemies, blocks, items);
	this->move();

	this->resetForce();
}

void Player::collision(std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items)
{
	Direction collisionDirection = Direction::notD;
	Vec2 forcedMoveVector;

	this->isOnGround = false;
	for (Block i : blocks)
	{
		if (this->collisionCheck(i, collisionDirection, forcedMoveVector))
		{
			switch (collisionDirection)
			{
			case Direction::up:
				this->isOnGround = true;
				break;
			case Direction::down:
				break;
			case Direction::left:
				break;
			case Direction::right:
				break;
			}

			this->collisionInteract(collisionDirection);
			collisionDirection = Direction::notD;
		}
	}
	std::cout << std::endl;

	for (int i = 0; i < enemies.size(); i++)
	{
		if (this->collisionCheck(enemies[i], collisionDirection, forcedMoveVector))
		{
			switch (collisionDirection)
			{
			case Direction::up:
				break;
			case Direction::down:
				break;
			case Direction::left:
				break;
			case Direction::right:
				break;
			}
			Game.CurrentScreen = Reload;
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
			items[i].interact(*this);

			items.erase(items.begin() + i);
			--i;
			collisionDirection = Direction::notD;
		}
	}

	this->forcedMove(forcedMoveVector);
}



void Player::draw(Camera camera)
{
	using namespace doodle;

	const float& magnification = (float)camera.getMagnification();
	const float thisX = (float)(this->getPosition().xValue() - camera.getPosition().xValue()) * magnification;
	const float thisY = (float)(this->getPosition().yValue() - camera.getPosition().yValue()) * magnification;

	push_settings();
	set_fill_color(HexColor{ this->getColor() });
	draw_rectangle(thisX, thisY, (float)this->getWidth() * magnification, (float)this->getHeight() * magnification);
	if (this->reloadTime != 0)
		draw_rectangle(thisX, thisY + (float)this->getHeight(), (float)(RELOADTIME - this->reloadTime) / 50.f * magnification, 10.f * magnification);
	for (int i = 0; i < this->magazine; i++)
		draw_rectangle(thisX - (float)this->getHeight() / 10  + ((float)this->getWidth() / 2 * i) ,
						thisY ,
						(float)this->getHeight()/ 4);
	pop_settings();
}