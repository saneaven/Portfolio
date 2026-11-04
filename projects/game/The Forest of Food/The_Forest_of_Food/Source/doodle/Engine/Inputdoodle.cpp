/*
File name : Inputdoodle.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "doodle/input.hpp"
#include "Input.h"
#include "Engine.h"

Input::KeyboardButton DoodleKeyToCS230Key(doodle::KeyboardButtons button) {
	if (button == doodle::KeyboardButtons::Enter) {
		return Input::KeyboardButton::Enter;
	}
	else if (button == doodle::KeyboardButtons::Escape) {
		return Input::KeyboardButton::Escape;
	}
	else if (button == doodle::KeyboardButtons::Space) {
		return Input::KeyboardButton::Space;
	}
	else if (button == doodle::KeyboardButtons::Left) {
		return Input::KeyboardButton::Left;
	}
	else if (button == doodle::KeyboardButtons::Right) {
		return Input::KeyboardButton::Right;
	}
	else if (button == doodle::KeyboardButtons::Up) {
		return Input::KeyboardButton::Up;
	}
	else if (button == doodle::KeyboardButtons::Down) {
		return Input::KeyboardButton::Down;
	}
	else if(button == doodle::KeyboardButtons::Backspace) {
		return Input::KeyboardButton::Backspace;
	}
	else if(button == doodle::KeyboardButtons::CapsLock) {
		return Input::KeyboardButton::CapsLock;
	}
	else if(button == doodle::KeyboardButtons::Control) {
		return Input::KeyboardButton::Control;
	}
	else if(button == doodle::KeyboardButtons::Shift) {
		return Input::KeyboardButton::Shift;
	}
	else if (button >= doodle::KeyboardButtons::A && button <= doodle::KeyboardButtons::Z) {
		int offset = static_cast<int>(button) - static_cast<int>(doodle::KeyboardButtons::A);
		return static_cast<Input::KeyboardButton>(static_cast<int>(Input::KeyboardButton::A) + offset);
	}
	return Input::KeyboardButton::None;
}

void on_key_pressed(doodle::KeyboardButtons doodleButton) {
	Input::KeyboardButton button = DoodleKeyToCS230Key(doodleButton);
	if (button != Input::KeyboardButton::None) {

		Engine::GetInput().SetKeyDown(button, true);
	}
}

void on_key_released(doodle::KeyboardButtons doodleButton) {
	Input::KeyboardButton button = DoodleKeyToCS230Key(doodleButton);
	if (button != Input::KeyboardButton::None) {

		Engine::GetInput().SetKeyDown(button, false);
	}
}