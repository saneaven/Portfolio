/*
File name : Vector2D.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <cmath>

template <typename T = double >

struct Vector2D
{
public:
	T x;
	T y;

	static_assert(std::is_arithmetic_v<T>, "T should be able to do math stuff like int/double");

	bool isReal = true;

	constexpr Vector2D() noexcept : x(0), y(0) {};
	constexpr Vector2D(T x, T y) noexcept : x(x), y(y) {};
	constexpr explicit Vector2D(bool boolian) noexcept : isReal(boolian) {};
	constexpr Vector2D(const Vector2D<T>& vector) noexcept = default;

	constexpr Vector2D<T>& operator=(const Vector2D<T>& vector) noexcept = default;

	[[nodiscard]] constexpr bool operator==(const Vector2D<T>& vector) const noexcept;
	[[nodiscard]] constexpr bool operator!=(const Vector2D<T>& vector) const noexcept { return !(*this == vector); }

	constexpr Vector2D<T>& operator+=(const Vector2D<T>& vector);
	constexpr Vector2D<T>& operator-=(const Vector2D<T>& vector);
	constexpr Vector2D<T>& operator*=(const T scalar);
	constexpr Vector2D<T>& operator/=(const T scalar);


	[[nodiscard]] constexpr const Vector2D<T> operator-() const noexcept { return Vector2D(*this) *= -1; }
	[[nodiscard]] constexpr const Vector2D<T> operator+(const Vector2D<T>& vector) const noexcept { return Vector2D<T>(*this) += vector; }
	[[nodiscard]] constexpr const Vector2D<T> operator-(const Vector2D<T>& vector) const noexcept { return Vector2D<T>(*this) -= vector; }
	[[nodiscard]] constexpr const Vector2D<T> operator*(const T& scalar) const noexcept { return Vector2D<T>(*this) *= scalar; }
	[[nodiscard]] constexpr const Vector2D<T> operator/(const T& scalar) const noexcept { return Vector2D<T>(*this) /= scalar; }

	template <typename Q>
	[[nodiscard]] constexpr operator Vector2D<Q>() const noexcept { return Vector2D<Q>(static_cast<Q>(x), static_cast<Q>(y)); }

	[[nodiscard]] const double GetSquaredLength() const noexcept { return x * x + y * y; }
	[[nodiscard]] const double GetLength() const noexcept { return sqrt(x * x + y * y); }
	[[nodiscard]] const double GetAngle() const noexcept { return atan2(y, x); }
	[[nodiscard]] const Vector2D<T> Normalize() const noexcept { return *this / GetLength(); }

	[[nodiscard]] const double dotProduct(const Vector2D<T>& other) const { return (this->x * other.x) + (this->y * other.y); }
	[[nodiscard]] const double crossProduct(const Vector2D<T>& other) const { return (this->x * other.y) - (this->y * other.x); }

	[[nodiscard]] const double distance(const Vector2D<T>& other) const { return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2)); }

	void SetLength(const double length) noexcept;
	void SetAngle(const double radian) noexcept;

	template <typename Q>
	[[nodiscard]] constexpr const Vector2D<Q> convert() const noexcept { return Vector2D<Q>(static_cast<Q>(x), static_cast<Q>(y)); }
};

#include "Vector2D.inl"