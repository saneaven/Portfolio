/*
File name : basicFunc.h
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Haneul Lee
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#pragma once
#include "SFX.h"
#include "classes.h"
#include <functional>

extern int notePosX;
extern int notePosY;

extern int noteSize;

extern bool is_space_pressed;
extern int GateNoteNum;


extern std::vector<Sonar> sonars;
extern std::vector<HUD> HUDs;
extern GAME Game;

void updateConstHUD();

bool GetIntersectPoint(const Vec2& aP1, const Vec2& aP2, const Vec2& bP1, const Vec2& bP2, Vec2& IP);
bool isParallelIntersect(const Vec2& a1, const Vec2& a2, const Vec2& b1, const Vec2& b2);

void setAnimation(double* target, double start, double end, int milisec);
bool deleteAnimation(double* target);
void animationUpdate();

void setTimer(std::function<void()> func, int millisec);
void timerUpdate();

void make_cursor();

void drawHUD(Player player);
void initHUD();
void UpdateHUD(Player player);