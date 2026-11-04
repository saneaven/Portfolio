/*
File name : Sprite.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <string>			
#include "Animation.h"
#include "Texture.h"			


class Sprite {
public:
    Sprite();
    void Load(std::string spriteInfoFile);
    void Update(double dt);
    void Draw(TransformMatrix displayMatrix);
    void PlayAnimation(int anim);
    bool IsAnimationDone();
    void AddHotSpot(Vector2D<int> position);
    Vector2D<int> GetHotSpot(unsigned int index);
    Vector2D<int> GetFrameSize() const;
    Vector2D<int> GetFrameTexel(int frameNum) const;
private:
    Texture *texture;
    int currAnim;
    Vector2D<int> frameSize;
    std::vector<Vector2D<int>> frameTexel;
    std::vector<Animation> animations;
    std::vector<Vector2D<int>> hotspots;

};
