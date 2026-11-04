/*
File name : Transform.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(Secondary) :Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Transform.h"


TransformMatrix::TransformMatrix() {

	Reset();
}

void TransformMatrix::Reset() {


	matrix[0][0] = 1.0; 	matrix[0][1] = 0.0; 	matrix[0][2] = 0.0;
	matrix[1][0] = 0.0; 	matrix[1][1] = 1.0; 	matrix[1][2] = 0.0;
	matrix[2][0] = 0.0; 	matrix[2][1] = 0.0; 	matrix[2][2] = 1.0;
}

TranslateMatrix::TranslateMatrix(Vector2D<int> translate) {
	matrix[0][0] = 1; 	matrix[0][1] = 0; 	matrix[0][2] = translate.x;
	matrix[1][0] = 0; 	matrix[1][1] = 1; 	matrix[1][2] = translate.y;
	matrix[2][0] = 0; 	matrix[2][1] = 0; 	matrix[2][2] = 1;
}

TranslateMatrix::TranslateMatrix(Vector2D<> translate) {
	matrix[0][0] = 1.0; 	matrix[0][1] = 0.0; 	matrix[0][2] = translate.x;
	matrix[1][0] = 0.0; 	matrix[1][1] = 1.0; 	matrix[1][2] = translate.y;
	matrix[2][0] = 0.0; 	matrix[2][1] = 0.0; 	matrix[2][2] = 1.0;
}

RotateMatrix::RotateMatrix(double radians) {
	double sin = std::sin(radians);
	double cos = std::cos(radians);
	matrix[0][0] = cos; 	matrix[0][1] = -sin; 	matrix[0][2] = 0.0;
	matrix[1][0] = sin; 	matrix[1][1] = cos; 	matrix[1][2] = 0.0;
	matrix[2][0] = 0.0; 	matrix[2][1] = 0.0; 	matrix[2][2] = 1.0;
}

ScaleMatrix::ScaleMatrix(Vector2D<> scale) {
	matrix[0][0] = scale.x; 	matrix[0][1] = 0.0; 	matrix[0][2] = 0.0;
	matrix[1][0] = 0.0; 	matrix[1][1] = scale.y; 	matrix[1][2] = 0.0;
	matrix[2][0] = 0.0; 	matrix[2][1] = 0.0; 	matrix[2][2] = 1.0;
}

TransformMatrix TransformMatrix::operator * (TransformMatrix rhs) const {
	TransformMatrix result;

	result[0][0] = matrix[0][0] * rhs[0][0] + matrix[0][1] * rhs[1][0] + matrix[0][2] * rhs[2][0];
	result[0][1] = matrix[0][0] * rhs[0][1] + matrix[0][1] * rhs[1][1] + matrix[0][2] * rhs[2][1];
	result[0][2] = matrix[0][0] * rhs[0][2] + matrix[0][1] * rhs[1][2] + matrix[0][2] * rhs[2][2];
	result[1][0] = matrix[1][0] * rhs[0][0] + matrix[1][1] * rhs[1][0] + matrix[1][2] * rhs[2][0];
	result[1][1] = matrix[1][0] * rhs[0][1] + matrix[1][1] * rhs[1][1] + matrix[1][2] * rhs[2][1];
	result[1][2] = matrix[1][0] * rhs[0][2] + matrix[1][1] * rhs[1][2] + matrix[1][2] * rhs[2][2];
	result[2][0] = matrix[2][0] * rhs[0][0] + matrix[2][1] * rhs[1][0] + matrix[2][2] * rhs[2][0];
	result[2][1] = matrix[2][0] * rhs[0][1] + matrix[2][1] * rhs[1][1] + matrix[2][2] * rhs[2][1];
	result[2][2] = matrix[2][0] * rhs[0][2] + matrix[2][1] * rhs[1][2] + matrix[2][2] * rhs[2][2];

	return result;
}

Vector2D<> TransformMatrix::operator * (Vector2D<> rhs) const {
	Vector2D result;
	result.x = matrix[0][0] * rhs.x + matrix[0][1] * rhs.y;
	result.y = matrix[1][0] * rhs.x + matrix[1][1] * rhs.y;
	return result;
}

TransformMatrix& TransformMatrix::operator *= (TransformMatrix rhs) {
	(*this) = (*this) * rhs;
	return (*this);
}