/*
File name : Animation.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include <vector>  //std::vector
#include <string>  //std::string



class Animation {
public:
    Animation();

    Animation(std::string file);
    void Update(double dt);
    int GetDisplayFrame();
    void Reset();
    bool IsAnimationDone();
private:
    enum class Command {
        PlayFrame,
        Loop,
        End,
    };
    class CommandData {
    public:
        virtual Command GetType() = 0;
    };
    class PlayFrame : public CommandData {
    public:
        PlayFrame(int frame, double duration);
        Command GetType() override { return Command::PlayFrame; }
        void Update(double dt);
        bool IsFrameDone();
        void Reset();
        int GetFrameNum();
    private:
        int frame;
        double targetTime;
        double timer;
    };
    class Loop : public CommandData {
    public:
        Loop(int loopToIndex);
        Command GetType() override { return Command::Loop; }
        int GetLoopToIndex();
    private:
        int loopToIndex;
    };
    class End : public CommandData {
    public:
        Command GetType() override { return Command::End; }
    private:
    };

    bool isAnimationDone;
    int animCommandIndex;
    PlayFrame* currPlayFrameData;
    std::vector<CommandData*> animation;
};