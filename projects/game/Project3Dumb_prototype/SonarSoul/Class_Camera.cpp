/*
File name : Class_Camera.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#include "classes.h"
#include <iostream>


Camera::Camera(Vec2 pos) : Object(pos) {}
Camera::Camera(Object* const fol) : Object(fol->getPosition()), follow(fol) {}
Camera::Camera(Object* const fol, double mag) : Object(fol->getPosition()), follow(fol), magnification(mag) {}

double Camera::getMagnification() {return this->magnification; }
void Camera::changeMagnification(double mag) { magnification = mag; }
//void Camera::update() {  }
void Camera::update() { this->setPosition(this->follow->getPosition()); }