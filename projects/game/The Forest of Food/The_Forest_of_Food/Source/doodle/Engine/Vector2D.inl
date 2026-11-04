/*
File name : Vector2D.inl
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#include "Vector2D.h"
#include <limits>
#include <iostream>

template <typename T>
[[nodiscard]] constexpr bool Vector2D<T>::operator==(const Vector2D<T>& vector) const noexcept
{
	return ((-(std::numeric_limits <T> ::epsilon()) <= x - vector.x && x - vector.x <= std::numeric_limits <T> ::epsilon())
		&& (-(std::numeric_limits <T> ::epsilon()) <= y - vector.y && y - vector.y <= std::numeric_limits <T> ::epsilon()));
} 

template <typename T>
constexpr Vector2D<T>& Vector2D<T>::operator+=(const Vector2D<T>& vector)
{
	x += vector.x;
	y += vector.y;
	return *this;
}

template <typename T>
constexpr Vector2D<T>& Vector2D<T>::operator-=(const Vector2D<T>& vector)
{
	x -= vector.x;
	y -= vector.y;
	return *this;
}

template <typename T>
constexpr Vector2D<T>& Vector2D<T>::operator*=(const T scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

template <typename T>
constexpr Vector2D<T>& Vector2D<T>::operator/=(const T scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

template <typename T>
void Vector2D<T>::SetLength(const double length) noexcept 
{ 
	double tempAngle = GetAngle();
	x = cos(tempAngle) * length;
	y = sin(tempAngle) * length;
}

template <typename T>
void Vector2D<T>::SetAngle(const double radian) noexcept
{
	double tempLength = GetLength();
	x = tempLength * cos(radian);
	y = tempLength * sin(radian);
}


template <typename T>
std::ostream& operator<<(std::ostream& output, const Vector2D<T>& vector)
{
	output << "{ " << vector.x << ", " << vector.y << " }";
	return output;
}