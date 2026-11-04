/*
	File name: Class_Button.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	Author: Haeun Park
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <doodle/doodle.hpp>
#include <string>

#include "classes.h"

Button::Button(std::string tex, float x, float y, float width, float height) : text{ tex }, pos_x{ x }, pos_y{ y }, button_width{ width }, button_height{ height } {}
Button::Button(std::string tex, float x, float y, float width, float height, float fnt_size) : text{ tex }, pos_x{ x }, pos_y{ y }, button_width{ width }, button_height{ height } {setFontSize(fnt_size);}

bool Button::isMouseOnButton()
{
	if (doodle::get_mouse_x() < pos_x + button_width / 2 && doodle::get_mouse_x() > pos_x - button_width / 2 && doodle::get_mouse_y() < pos_y + button_height / 2 && doodle::get_mouse_y() > pos_y - button_height / 2)
		return true;
	else
		return false;
}

void Button::draw()
{
	doodle::push_settings();
	if (this->isMouseOnButton())
	{
		doodle::set_fill_color(180);
	}
	else
		doodle::set_fill_color(255);
	doodle::set_rectangle_mode(doodle::RectMode::Center);
	doodle::set_font_size(fnt_size);
	doodle::draw_rectangle(pos_x, pos_y, button_width, button_height);
	doodle::draw_text(text, pos_x - text.size() / 2 * (fnt_size - 10), pos_y - fnt_size + 5);
	doodle::pop_settings();
}

void Button::setFontSize(float size)
{
	this->fnt_size = size;
}
