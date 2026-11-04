/*
File name : HUD.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include <cmath>
#include <doodle/doodle.hpp>
#include "classes.h"
#include "basicFunc.h"

using namespace std;

int notePosX;
int notePosY;

int noteSize = 30;

HUD::HUD(string n, Vec2 pos, double w) : position(pos), name(n), width(w), height(w) { color = 0xFFFFFF88; }
HUD::HUD(string n, Vec2 pos, double w, double h) : position(pos), name(n), width(w), height(h) {}

const Vec2 HUD::getPosition() const { return this->position; }
void HUD::setPosition(const Vec2& pos) { this->position = pos; }

void HUD::moveAnimation(Vec2 start, Vec2 end, int milisec)
{
	this->isAnimation = true;
	setAnimation(&(this->position.x), start.x, end.x, milisec);
	setAnimation(&(this->position.y), start.y, end.y, milisec);

	setTimer([this]() { this->isAnimation = false; }, milisec);
}

void HUD::draw()
{
	using namespace doodle;
	
	push_settings();
	set_fill_color(HexColor{ this->color });
	set_font_size((float)this->height);
	draw_rectangle((float)this->position.x, (float)this->position.y, (float)this->width, (float)this->height);
	draw_text(this->name, (float)this->position.x - (float)this->width / 2.f , (float)this->position.y - (float)this->height);
	pop_settings();
}


vector<HUD> HUDs;

void updateConstHUD()
{
	notePosX = (-doodle::Width / 2) + doodle::Width / 10;
	notePosY = doodle::Height / 2 - doodle::Height / 6;
}


void initHUD()
{
	HUDs.push_back(HUD("R", Vec2(notePosX, notePosY), noteSize));
	HUDs.push_back(HUD("E", Vec2(notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("W", Vec2(notePosX, notePosY - noteSize * 2), noteSize));
	HUDs.push_back(HUD("Q", Vec2(notePosX, notePosY - noteSize * 3), noteSize));
	HUDs.push_back(HUD("F", Vec2(notePosX, notePosY - noteSize * 4), noteSize));
	HUDs.push_back(HUD("D", Vec2(notePosX, notePosY - noteSize * 5), noteSize));
	HUDs.push_back(HUD("S", Vec2(notePosX, notePosY - noteSize * 6), noteSize));
	HUDs.push_back(HUD("A", Vec2(notePosX, notePosY - noteSize * 7), noteSize));



	HUDs.push_back(HUD("Do", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("Re", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("Mi", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("Fa", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("Sol", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("La", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("Ti", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
	HUDs.push_back(HUD("High Do", Vec2(-(double)notePosX, notePosY - noteSize), noteSize));
}



HUD& searchHUD(string name)
{
	for (HUD& i : HUDs)
	{
		if (i.name == name)
			return i;
	}

	return HUDs[0];
}

int GateNoteNum = 0;

void drawHUD(Player player)
{
	for (int i = 0; i < 8; i++)
	{
		HUDs[i].draw();
	}

	HUDs[8 + (abs(GateNoteNum) % 8)].draw();
	doodle::draw_text(to_string(player.gateNotes[abs(GateNoteNum) % 8]), -(float)notePosX, (float)notePosY + noteSize);
}


void UpdateHUD(Player player)
{

	if (!is_space_pressed)
	{
		if (HUDs[0].isAnimation == false)
		{
			for (int i = 0; i < 8; i++)
			{
				HUDs[i].color = 0xFFFFFFFF;
				HUDs[7 - i].setPosition(Vec2(notePosX + noteSize * (i % 4), notePosY + noteSize * i / 4));
			}
		}

	}
	else
	{
		if (HUDs[0].isAnimation == false)
		{
			for (int i = 0; i < 8; i++)
			{
				HUDs[i].color = 0xFF0000FF;
				HUDs[i].setPosition(Vec2(notePosX, notePosY - noteSize * i + doodle::Height / 10));
			}
		}
	}

	for (int i = 8; i < 16; i++)
	{
		HUDs[i].color = 0x00FFFF88;
		HUDs[i].setPosition(Vec2(-notePosX, notePosY));
	}

	if (player.q_note == false)
		searchHUD("Q").color = 0x888888FF;
	if (player.w_note == false)
		searchHUD("W").color = 0x888888FF;
	if (player.e_note == false)
		searchHUD("E").color = 0x888888FF;
	if (player.r_note == false)
		searchHUD("R").color = 0x888888FF;
	if (player.a_note == false)
		searchHUD("A").color = 0x888888FF;
	if (player.s_note == false)
		searchHUD("S").color = 0x888888FF;
	if (player.d_note == false)
		searchHUD("D").color = 0x888888FF;
	if (player.f_note == false)
		searchHUD("F").color = 0x888888FF;


}