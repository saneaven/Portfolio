/*
File name : mousePointer.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : Haneul Lee
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include "basicFunc.h"
#include <doodle/doodle.hpp>

void make_cursor() 
{	
	using namespace doodle;
	push_settings();
	set_fill_color(Color4ub(0, 255, 0));
	draw_ellipse(static_cast<float>(doodle::get_mouse_x()), static_cast<float>(doodle::get_mouse_y()), 8);
	draw_rectangle(static_cast<float>(doodle::get_mouse_x() - 10), static_cast<float>(doodle::get_mouse_y()), 8, 5);
	draw_rectangle(static_cast<float>(doodle::get_mouse_x() + 10), static_cast<float>(doodle::get_mouse_y()), 8, 5);
	draw_rectangle(static_cast<float>(doodle::get_mouse_x()), static_cast<float>(doodle::get_mouse_y()) + 10, 5, 8);
	draw_rectangle(static_cast<float>(doodle::get_mouse_x()), static_cast<float>(doodle::get_mouse_y()) - 10, 5, 8);
	pop_settings();
}