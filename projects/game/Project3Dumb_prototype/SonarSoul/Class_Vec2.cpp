/*
File name : Class_Vec2.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Hoseob Jeong
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <doodle/doodle.hpp>
#include "classes.h"
#include "basicFunc.h"

//physical vector
Vec2& Vec2::operator+=(const Vec2& other) { this->x += other.x, this->y += other.y;  return *this; }
Vec2& Vec2::operator-=(const Vec2& other) { this->x -= other.x, this->y -= other.y;  return *this; }
Vec2& Vec2::operator*=(const double& scalar) { this->x *= scalar, this->y *= scalar;  return *this; }
Vec2& Vec2::operator/=(const double& scalar) { this->x /= scalar, this->y /= scalar;  return *this; }

const double Vec2::operator*(const Vec2& other) const { return (this->x * other.x) + (this->y * other.y); }
const double Vec2::dotProduct(const Vec2& other) const  {return (this->x * other.x) + (this->y * other.y); }
const double Vec2::crossProduct(const Vec2& other) const { return (this->x * other.y) - (this->y * other.x); }
const Vec2 Vec2::operator+(const Vec2& other)const { return Vec2(*this) += other; }
const Vec2 Vec2::operator-(const Vec2& other)const { return Vec2(*this) -= other; }
const Vec2 Vec2::operator-() const { return Vec2(0, 0) - Vec2(*this); }
const Vec2 Vec2::operator*(const double& scalar)const { return Vec2(*this) *= scalar; }
const Vec2 Vec2::operator/(const double& scalar)const { return Vec2(*this) /= scalar; }
const double Vec2::distance(const Vec2& other) const { return sqrt(pow(this->x - other.x,2) + pow(this->y - other.y, 2)); }

bool Vec2::isSimilarWith(const Vec2& other) const { return (abs(this->x - other.x) < 1) && (abs(this->y - other.y) < 1); }
bool Vec2::operator==(const Vec2& other) const { return (abs(this->x - other.x) <= FLT_EPSILON) && (abs(this->y - other.y) < FLT_EPSILON); }
bool Vec2::operator!=(const Vec2& other) const { return !(*this == other); }

bool Vec2::operator<(const Vec2& other) const 
{
	if ((this->x - FLT_EPSILON) < (other.x + FLT_EPSILON))
	{
		if ((this->y - FLT_EPSILON) < (other.y + FLT_EPSILON))
			return true;
	}
	return false;
}
bool Vec2::operator<=(const Vec2& other) const 
{
	if (*this < other || *this == other)
		return true;
	else
		return false;
}
bool Vec2::operator>(const Vec2& other) const 
{
	if ((this->x + FLT_EPSILON) > (other.x - FLT_EPSILON))
	{
		if ((this->y + FLT_EPSILON) > (other.y - FLT_EPSILON))
			return true;
	}
	return false;
}
bool Vec2::operator>=(const Vec2& other) const 
{
	if (*this > other || *this == other)
		return true;
	else
		return false;
}

double Vec2::getAngle() const
{
	if (this->getLength() != 0)
		return atan2(this->y, this->x);
	else
		return 0;
}
void Vec2::setAngle(double radian)
{
	double length = this->getLength();
	this->x = length * cos(radian);
	this->y = length * sin(radian);
}

double Vec2::getSlope() const
{
	if (this->getLength() != 0)
		return this->y / this->x;
	else
		return 0;
}


double Vec2::getLength() const { return sqrt((this->x * this->x) + (this->y * this->y)); }
void Vec2::setLength(double length)
{
	double angle = this->getAngle();
	this->x = length * cos(angle);
	this->y = length * sin(angle);
};
void Vec2::addLength(double length) { this->setLength(this->getLength() + length); };

double Vec2::xValue() const { return this->x; }
double Vec2::yValue() const { return this->y; }

void Vec2::print() { std::cout << "(" << this->xValue() << ", " << this->yValue() << ")" << std::endl; }
std::ostream& operator<<(std::ostream& outstream, Vec2& vector)
{
	outstream << "(" << vector.xValue() << ", " << vector.yValue() << ")" << std::endl;
	return outstream;
}

void Vec2::draw(const Vec2& from) 
{
	doodle::draw_line((float)from.xValue(), (float)from.yValue(), (float)this->x, (float)this->y);
}

/*
//example
int main(void)
{
	vec2 v(3, 1);
	vec2 v2(1, 2);

	v += v2;

	v.print();
	v2.print();
	v2(v + v2).print();

	v = v2;
	v.print();

	std::cout << v.getAngle() << std::endl;
	std::cout << v.getLength() << std::endl;

	if (v == v2)
		std::cout << "both are equal!" << std::endl;

}
*/
