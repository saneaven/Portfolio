/*
	File name: Class_Item.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	Author: Minjae Kyung
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include "classes.h"
#include <iostream>


Camera::Camera(Vec2 pos) : Object(pos) {}
Camera::Camera(Object* const fol) : Object(fol->getPosition()), follow(fol) {}
Camera::Camera(Object* const fol, double mag) : Object(fol->getPosition()), follow(fol), magnification(mag) {}

double Camera::getMagnification() {return this->magnification; }
void Camera::changeMagnification(double mag) { magnification = mag; }
void Camera::update() { this->setPosition(this->follow->getPosition()); }




Item::Item(Vec2 p, int t) : Object(p, 10, 10, 1), type(t)
{
	switch (t)
	{
	case 0:
		this->setColor(0xFFFFFFFF);
		break;
	case 1:
		this->setColor(0xFF0000FF);
		break;
	case 2:
		this->setColor(0x00FF00FF);
		break;
	case 3:
		this->setColor(0x0000FFFF);
		break;
	case 4:
		this->setColor(0x0000000FF);
		break;

	case 10:
		this->setColor(0xAAAA00FF);
		break;
	}
}

void Item::interact(Player& player)
{
	if (this->type == 10)
		player.isStageClear = true;
	else
		player.setColor(this->getColor());
}