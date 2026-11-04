/*
File name : MouseClick.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "doodle/doodle.hpp"
#include "Object.h"

class MouseClick: Object {
public:
	MouseClick() : Object({ 0, 0 }, { 0, 0 }, {{0, 0}, {0, 0}}, "assets/mouse_s.spt", "mouse") {
		AddSFX("Click", "sounds/click.ogg");
	}

	void Update(GameObjectManager&, double) override {}
	bool MouseOneClick() {
		if (doodle::MouseIsPressed && breakMouseClick) {
			MouseRelese = true;
		}
		else if (doodle::MouseIsPressed == false) {
			breakMouseClick = true;
		}
		if (MouseRelese) {
			PlaySFX("Click");
			breakMouseClick = false;
			MouseRelese = false;
			return true;
		}
		return false;
	}
	
private:
	bool MouseRelese = false;
	bool breakMouseClick = true;
};