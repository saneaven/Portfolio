/*
File name : Sprite.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
Author(Second):Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Sprite.h"
#include "Engine.h"
#include "Transform.h"
#include <fstream>

Sprite::Sprite() :texture(nullptr),currAnim(0) {};



void Sprite::Load(std::string spriteInfoFile) {
    animations.clear();
    hotspots.clear();
    
    if (spriteInfoFile.substr(spriteInfoFile.find_last_of('.')) != ".spt") {
        throw std::runtime_error("Bad Filetype.  " + spriteInfoFile + " not a sprite info file (.spt)");
    }
    std::ifstream inFile(spriteInfoFile);

    if (inFile.is_open() == false) {
        throw std::runtime_error("Failed to load " + spriteInfoFile);
    }

    std::string text;
    inFile >> text;
    texture = Engine::GetTextureManager().Load(text);
    frameSize = texture->GetSize();
    while (inFile.eof() == false) {
        inFile >> text;
        if (text == "FrameSize") {
            inFile >> frameSize.x;
            inFile >> frameSize.y;
        }
        else if (text == "Frame") {
            int frameLocationX, frameLocationY;
            inFile >> frameLocationX;
            inFile >> frameLocationY;
            frameTexel.push_back({ frameLocationX, frameLocationY });
        }
        else if (text == "HotSpot") {
            int hotSpotX, hotSpotY;
            inFile >> hotSpotX;
            inFile >> hotSpotY;
            hotspots.push_back({ hotSpotX, hotSpotY });
        }
        else if (text == "Anim") {
            inFile >> text;
            Animation newAnim(text);
            animations.push_back(newAnim);
        }
        else {
            
            throw "Unknown spt Commnad";
        }
    }
    if (frameTexel.empty() == true) {
        frameTexel.push_back({ 0,0 });
    }
    if (animations.empty() == true) {
        animations.push_back(Animation());
        PlayAnimation(0);
    }
}

void Sprite::Update(double dt)
{
    animations[currAnim].Update(dt);
}

void Sprite::Draw(TransformMatrix displayMatrix)
{
    texture->Draw(displayMatrix * TranslateMatrix(-hotspots[0]),
        GetFrameTexel(animations[currAnim].GetDisplayFrame()), GetFrameSize());
}


void Sprite::AddHotSpot(Vector2D<int> position)
{
    hotspots.push_back(position);
}

Vector2D<int> Sprite::GetHotSpot(unsigned int index)
{
    if (index >= hotspots.size())
    {
        throw "Invaild hotspot index";
      
    }
    else
    {
        return hotspots[index];
    }

}

        
Vector2D<int> Sprite::GetFrameSize() const
{
    return frameSize;
}

Vector2D<int> Sprite::GetFrameTexel(int frameNum) const
{
    if (frameNum >= frameTexel.size() || frameNum < 0) {

        return { 0,0 };
    }
    else {

        return frameTexel[frameNum];
    }

}

bool Sprite::IsAnimationDone()
{
    return animations[currAnim].IsAnimationDone();
}

void Sprite::PlayAnimation(int anim)
{

    if (anim <= animations.size()-1 && anim >= 0)
    {
        currAnim = anim;
        animations[currAnim].Reset();
    }

}





