/*
File name : Animation.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Kevin Wright,Sieun Choi
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include <fstream>    // file input stuff & runtime_error
#include "../Engine/Engine.h"    // Engine::GetLogger
#include "Animation.h"


Animation::Animation() : Animation("assets/none.anm") { }

Animation::Animation(std::string fileName) : animCommandIndex(0), currPlayFrameData(nullptr), isAnimationDone(false) {

    std::ifstream inFile(fileName);
    std::string label;

    if (fileName.substr(fileName.find_last_of('.')) != ".anm") {
        throw std::runtime_error("Bad Filetype.  " + fileName + " not a sprite info file (.anm)");
    }
    if (inFile.is_open() == false) {
        throw std::runtime_error("Failed to load " + fileName);
    }

    while (inFile.eof() == false) {
        inFile >> label;
        if (label == "PlayFrame") {
            int frame;
            float targetTime;
            inFile >> frame;
            inFile >> targetTime;

            animation.push_back(new PlayFrame(frame, targetTime));
        }
        else if (label == "Loop") {
            int loopToIndex;
            inFile >> loopToIndex;
            animation.push_back(new Loop(loopToIndex));
        }
        else if (label == "End") {
            animation.push_back(new End());
        }
        else {
            throw "Unknown Command" + label + "in anm file" + fileName;
        }
    }
    Reset();
}

void Animation::Update(double dt) {

    currPlayFrameData->Update(dt);

    if (currPlayFrameData->IsFrameDone() == true) {

        currPlayFrameData->Reset();

        ++animCommandIndex;

        if (animation[animCommandIndex]->GetType() == Command::PlayFrame) {
            currPlayFrameData = static_cast<PlayFrame*>(animation[animCommandIndex]);
        }
        else if (animation[animCommandIndex]->GetType() == Command::Loop) {
            Loop* loopData = static_cast<Loop*>(animation[animCommandIndex]);

            animCommandIndex = loopData->GetLoopToIndex();

            if (animation[animCommandIndex]->GetType() == Command::PlayFrame) {
                currPlayFrameData = static_cast<PlayFrame*>(animation[animCommandIndex]);
            }
            else {
                Reset();
                throw "Invaild Value";
                
            }
        }
        else if (animation[animCommandIndex]->GetType() == Command::End) {
            isAnimationDone = true;
            animation.clear();
        }
    }
}

int Animation::GetDisplayFrame()
{
    return currPlayFrameData->GetFrameNum();
}

void Animation::Reset()
{
    animCommandIndex = 0;
    currPlayFrameData = static_cast<PlayFrame*>(animation[animCommandIndex]);
    isAnimationDone = false;

}

bool Animation::IsAnimationDone()
{
    return isAnimationDone;
}


int Animation::PlayFrame::GetFrameNum()
{
    return frame;
}

bool Animation::PlayFrame::IsFrameDone()
{
    if (timer >= targetTime)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Animation::PlayFrame::PlayFrame(int frame, double duration) :frame(frame), targetTime(duration)
{
    timer = 0;
}

void Animation::PlayFrame::Reset()
{
    timer = 0;

}
void Animation::PlayFrame::Update(double dt)
{
    timer += dt;

}

int Animation::Loop::GetLoopToIndex()
{
    return loopToIndex;

}
Animation::Loop::Loop(int loopToIndex) :loopToIndex(loopToIndex)
{}








