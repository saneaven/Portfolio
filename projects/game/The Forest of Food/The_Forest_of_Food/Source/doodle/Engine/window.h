/*
File name : Window.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(secondary):Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "string"
#include "BasicDataTypes.h"

typedef unsigned Color;

class Window {
public:
    void Init(std::string windowName);
    void Update();
    void Resize(int newWidth, int newHeight);
    void Clear(Color color);
    Vector2D<int> GetSize();


private:

    Vector2D<int> screenSize;

};