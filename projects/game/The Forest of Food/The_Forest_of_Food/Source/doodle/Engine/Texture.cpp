/*
File name : Texture.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(Second):Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Texture.h"
#include <doodle\drawing.hpp>
#include "Transform.h"




Texture::Texture(const std::filesystem::path& filePath) : image(filePath) {
    size = { image.GetWidth(), image.GetHeight() };
}

Texture::Texture(const std::filesystem::path& filePath, Vector2D<> pos = {0, 0}) : image(filePath),pos(pos)  {
    size = { image.GetWidth(), image.GetHeight() };
}

void Texture::Draw(TransformMatrix displayMatrix) {

    doodle::push_settings();
    doodle::smooth_drawing();
    doodle::apply_matrix
    (
        displayMatrix[0][0], displayMatrix[1][0],
        displayMatrix[0][1], displayMatrix[1][1],
        displayMatrix[0][2], displayMatrix[1][2]
    );
    doodle::draw_image(image, 0, 0);
    doodle::pop_settings();
}

void Texture::Draw(TransformMatrix displayMatrix, Vector2D<int> texelPos, Vector2D<int> frameSize) {
    doodle::push_settings();
    doodle::smooth_drawing();
    doodle::apply_matrix(displayMatrix[0][0], displayMatrix[1][0], displayMatrix[0][1], displayMatrix[1][1], displayMatrix[0][2], displayMatrix[1][2]);
    doodle::draw_image(image, 0, 0, static_cast<double>(frameSize.x), static_cast<double>(frameSize.y), texelPos.x, texelPos.y);
    doodle::pop_settings();
}

void Texture::Draw() {
    doodle::smooth_drawing();
    doodle::draw_image(image, pos.x, pos.y);
}


Vector2D<int> Texture::GetSize() { return size; }

void Texture::SetPos(Vector2D<> position) {
    pos = position;
}

void Texture::Draw(Vector2D<>loca){

    doodle::push_settings();
    doodle::smooth_drawing();
    doodle::draw_image(image, loca.x,loca.y);
    doodle::pop_settings();
}