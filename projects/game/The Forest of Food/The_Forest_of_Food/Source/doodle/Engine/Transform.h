/*
File name : Transform.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(Secondary) :Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "BasicDataTypes.h"

class TransformMatrix {
public:
	TransformMatrix();

	double* operator[](int index) { return matrix[index]; }
	const double* operator[](int index) const { return matrix[index]; }
	TransformMatrix operator * (TransformMatrix rhs) const;
	Vector2D<> operator * (Vector2D<> rhs) const;
	TransformMatrix& operator *= (TransformMatrix rhs);
	void Reset();
protected:
	double matrix[3][3];
};

class TranslateMatrix : public TransformMatrix {
public:
	TranslateMatrix(Vector2D<int> translate);
	TranslateMatrix(Vector2D<> translate);
};

class RotateMatrix : public TransformMatrix {
public:
	RotateMatrix(double radians);
};

class ScaleMatrix : public TransformMatrix {
public:
	ScaleMatrix(Vector2D<> scale);
};