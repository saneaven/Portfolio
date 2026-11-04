/*
	File name: Class_Object.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	primary author: Minjae Kyung
	secondary author: Haeun Park
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <iostream>
#include <doodle/doodle.hpp>

#include "classes.h"

const Vec2 Object::getPosition() const { return this->position; }
const Vec2 Object::getVelocity() const { return this->velocity * doodle::DeltaTime; }
const Vec2 Object::getLawVelocity() const { return this->velocity; }
const double Object::getSpeed() const { return this->velocity.getLength() * doodle::DeltaTime; }
const Vec2 Object::getAccelaration() const { return this->accelaration; }
const Vec2 Object::getForce() const { return this->force; }
const double Object::getWidth() const { return this->width; }
const double Object::getHeight() const { return this->height; }
const double Object::getMass() const { return this->mass; }

const Vec2 Object::getPositionOf(Direction direction) const
{
	switch (direction)
	{
	case Direction::upLeft:
		return Vec2(this->getPosition().xValue() - (this->getWidth() / 2), this->getPosition().yValue() + (this->getHeight() / 2));
	case Direction::upRight:
		return Vec2(this->getPosition().xValue() + (this->getWidth() / 2), this->getPosition().yValue() + (this->getHeight() / 2));
	case Direction::downRight:
		return Vec2(this->getPosition().xValue() + (this->getWidth() / 2), this->getPosition().yValue() - (this->getHeight() / 2));
	case Direction::downLeft:
		return Vec2(this->getPosition().xValue() - (this->getWidth() / 2), this->getPosition().yValue() - (this->getHeight() / 2));
	case Direction::left:
		return Vec2(this->getPosition().xValue() - (this->getWidth() / 2), this->getPosition().yValue());
	case Direction::right:
		return Vec2(this->getPosition().xValue() + (this->getWidth() / 2), this->getPosition().yValue());
	case Direction::up:
		return Vec2(this->getPosition().xValue(), this->getPosition().yValue() + (this->getHeight() / 2));
	case Direction::down:
		return Vec2(this->getPosition().xValue(), this->getPosition().yValue() - (this->getHeight() / 2));
	default:
		std::cout << "error";
	}

	return Vec2(0, 0);
}

const uint32_t Object::getColor() const { return this->color; }
void Object::setColor(uint32_t s_color) { this->color = s_color; }
void Object::addColor(uint32_t a_color) { this->color += a_color; }
void Object::hitbyball(uint32_t col)
{
	bool isWhite = false;
	bool oneColor = false;
	bool twoColorMixed = false;

	if ((getColor() == 0xFFFFFFFF)) { isWhite = true; }
	else { isWhite = false; }
	if (getColor() == 0xFF0000FF || getColor() == 0x00FF00FF || getColor() == 0x0000FFFF) { oneColor = true; }
	else { oneColor = false; }
	if (getColor() == 0x7F7F00FF || getColor() == 0x7F007FFF || getColor() == 0x007F7FFF) { twoColorMixed = true; }
	else { twoColorMixed = false; }

	if (isWhite)
	{
		setColor(col);
	}
	else if (oneColor)
	{
		switch (getColor())
		{
		case 0xFF0000FF: // RED
			if (col == 0x00FF00FF) // + GREEN
				setColor(0x7F7F00FF);
			else if (col == 0x0000FFFF)   // + BLUE
				setColor(0x7F007FFF);
			break;

		case 0x00FF00FF: // GREEN
			if (col == 0xFF0000FF) // + RED
				setColor(0x7F7F00FF);
			else if (col == 0x0000FFFF)   // + BLUE
				setColor(0x007F7FFF);
			break;

		case 0x0000FFFF: // BLUE
			if (col == 0xFF0000FF) // + RED
				setColor(0x7F007FFF);
			else if (col == 0x00FF00FF)   // + GREEN
				setColor(0x007F7FFF);
			break;
		}
	}
	else if (twoColorMixed)
	{
		switch (getColor())
		{
		case 0x7F7F00FF: // RED + GREEN
			if (col == 0x0000FFFF) // + BLUE
				setColor(0x000000FF);
			break;

		case 0x7F007FFF: // RED + BLUE
			if (col == 0x00FF00FF) // + GREEN
				setColor(0x000000FF);
			break;

		case 0x007F7FFF: // GREEN + BLUE
			if (col == 0xFF0000FF) // + RED
				setColor(0x000000FF);
			break;
		}
	}
}

void Object::setWidth(double w) { this->width = w; }
void Object::setHeight(double h) { this->height = h; }
void Object::setPosition(Vec2 p) { this->position = p; }

void Object::move() { this->position += this->velocity * doodle::DeltaTime; }
void Object::moveTo(Vec2 pos) { this->position = pos; }
void Object::moveTo(double x, double y) { this->position = Vec2(x, y); }
void Object::forcedMove(Vec2 pos) { this->position += pos; }

void Object::accelerate() { this->velocity += this->accelaration * doodle::DeltaTime; }
void Object::setVelocity(Vec2 vel) { this->velocity = vel; }
void Object::setSpeed(double speed) { this->velocity.setLength(speed); }


void Object::setForceAccelation() { this->accelaration = (this->force / this->mass); }
void Object::gainAccelation(Vec2 accel) { this->accelaration += accel; }
void Object::setAccelaration(Vec2 accel) { this->accelaration = accel; }

void Object::gainForce(Vec2 f) { this->force += f; }
void Object::setForce(Vec2 f) { this->force = f; }
void Object::resetForce() { this->force = Vec2(0, 0); }

void Object::gravity() { if (this->isGravity) { this->force += Vec2(0, -G * this->mass); } }

bool Object::isPointInObject(Vec2 point) const
{
	if (this->getPositionOf(Direction::downLeft) <= point && point <= this->getPositionOf(Direction::upRight))
		return true;
	else
		return false;
}
bool Object::isPointInObject(double x, double y) const
{
	if (this->getPositionOf(Direction::left).xValue() < x && x < this->getPositionOf(Direction::right).xValue() &&
		this->getPositionOf(Direction::down).yValue() < y && y < this->getPositionOf(Direction::up).yValue())
		return true;
	else
		return false;
}


void Object::draw()
{
	using namespace doodle;
	const float& playerX = (float)this->getPosition().xValue();
	const float& playerY = (float)this->getPosition().yValue();

	push_settings();
	set_fill_color(HexColor{ this->getColor() });
	draw_rectangle(playerX, playerY, (float)this->getWidth(), (float)this->getHeight());
	pop_settings();
}

void Object::draw(Camera camera)
{
	using namespace doodle;
	
	const float& magnification = (float)camera.getMagnification();
	const float thisX = (float)(this->getPosition().xValue() - camera.getPosition().xValue()) * magnification;
	const float thisY = (float)(this->getPosition().yValue() - camera.getPosition().yValue()) * magnification;

	push_settings();
	set_fill_color(HexColor{ this->getColor() });
	draw_rectangle(thisX, thisY, (float)this->getWidth() * magnification, (float)this->getHeight() * magnification);
	pop_settings();
}