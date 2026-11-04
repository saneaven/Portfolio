/*
File name : Texture.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(Second):Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "BasicDataTypes.h"
#include "doodle/image.hpp"	// doodle::image

class TransformMatrix;

class Texture {
public:

    friend class TextureManager;
    Texture(const std::filesystem::path& filePath);
    Texture(const std::filesystem::path& filePath, Vector2D<> pos);
    void Draw(TransformMatrix displayMatrix);
    void Draw(TransformMatrix displayMatrix, Vector2D<int> texelPos, Vector2D<int> frameSize);
    void Draw(Vector2D<>);
    void Draw();
    Vector2D<int> GetSize();
    void SetPos(Vector2D<>position);
private:
    doodle::Image image;
    Vector2D<int> size;
    Vector2D<> pos;
};
