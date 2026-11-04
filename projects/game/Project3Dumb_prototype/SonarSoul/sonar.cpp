/*
File name : sonar.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung,  Hoseob Jeong
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include <doodle/doodle.hpp>
#include <iostream>
#include "classes.h"


Sonar::Sonar(Vec2 p, Vec2 v, Vec2 a, Vec2 f) : Object{0,0} {
	setVelocity(v); gainForce(f); isGravity = false;
	setAccelaration(a); setForce(f);
	setPosition(p);
	setColor(0xFF0000FF);
	syllable = Syllable::Do;
};

Sonar::Sonar(Vec2 p, double r, Syllable s) : Object{ p }, circle_radius(r), syllable(s){ isGravity = false; setColor(0xFF0000FF); };


void Sonar::draw(Camera camera)
{
	using namespace doodle;
	push_settings();
	set_fill_color(HexColor{ this->getColor() });
	no_outline();
	draw_ellipse((float)(this->getPosition().x - camera.getPosition().x)* (float)camera.getMagnification(), (float)(this->getPosition().y - camera.getPosition().y)* (float)camera.getMagnification(), (float)circle_radius*2 * (float)camera.getMagnification());
	pop_settings();
}

bool Sonar::update(std::vector<Enemy>& enemies)
{
	uint32_t color_c = this->getColor() & 0xFFFFFF00;
	this->alpha;
	this->circle_radius += SONARSPEED * doodle::DeltaTime;
	alpha -= SONARSPEED * doodle::DeltaTime;
	if (alpha < 0)
		return false;
	if(Circlecollision(enemies))
		return false;

	this->setColor(color_c | (int)alpha);
	return true;
}


double Sonar::get_circle_radius() {
	return this->circle_radius;
}

void Sonar::put_the_position(double x_value,double y_value) {

	this->setPosition(Vec2(x_value, y_value));
}

bool Sonar::Circlecollision(std::vector<Enemy>& enemies)
{
	Direction collisionDirection = Direction::notD;

	for (Enemy& i : enemies)
	{
		collisionDirection = Direction::notD;
	
		if (this->collisionCirRect(i, collisionDirection) && (this->syllable == i.getNote()))
		{
			i.setColor(0xFFAA00AA);
			break;
		}
	}

	return false;
}



bool Sonar::collisionCirRect(const Object& check, Direction& conllisionDirection)
{
	conllisionDirection = Direction::notD;

	const double& thisX = this->getPosition().xValue();
	const double& thisY = this->getPosition().yValue();


	const Vec2 checkA = check.getPositionOf(Direction::upLeft);
	const Vec2 checkB = check.getPositionOf(Direction::upRight);
	const Vec2 checkC = check.getPositionOf(Direction::downRight);
	const Vec2 checkD = check.getPositionOf(Direction::downLeft);
	const Vec2 checkE = check.getPositionOf(Direction::left);
	const Vec2 checkF = check.getPositionOf(Direction::right);
	const Vec2 checkG = check.getPositionOf(Direction::up);
	const Vec2 checkH = check.getPositionOf(Direction::down);

	const double max_rect_range_x = checkF.xValue() + this->get_circle_radius();
	const double min_rect_range_x = checkE.xValue() - this->get_circle_radius();
	const double max_rect_range_y = checkG.yValue() + this->get_circle_radius();
	const double min_rect_range_y = checkH.yValue() - this->get_circle_radius();
	if ((checkE.xValue() <= thisX && thisX <= checkF.xValue()) || (checkG.yValue() <= thisY && thisY <= checkH.yValue())) {



		if ((min_rect_range_x < thisX && thisX < max_rect_range_x) && (min_rect_range_y < thisY && thisY < max_rect_range_y))
		{
			return true;
		}




	}
	else
	{
		if (this->getPosition().distance(checkA) <= this->get_circle_radius())
			return true;
		if (this->getPosition().distance(checkB) <= this->get_circle_radius())
			return true;
		if (this->getPosition().distance(checkC) <= this->get_circle_radius())
			return true;
		if (this->getPosition().distance(checkD) <= this->get_circle_radius())
			return true;


	}





	return false;


}
