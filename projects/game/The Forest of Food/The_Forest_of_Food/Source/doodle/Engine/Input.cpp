/*
File name : Input.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/


#include "Input.h"
#include "Engine.h"

Input::Input() {
    keyDown.resize(static_cast<int>(KeyboardButton::Count));
    wasKeyDown.resize(static_cast<int>(KeyboardButton::Count));
}

bool Input::IsKeyDown(KeyboardButton key) const {
    return keyDown[static_cast<int>(key)];
}

bool Input::IsKeyUp(KeyboardButton key) const {
    return keyDown[static_cast<int>(key)] == false;
}

bool Input::IsKeyReleased(KeyboardButton key) const {
    return keyDown[static_cast<int>(key)] == false && wasKeyDown[static_cast<int>(key)] == true;
}

void Input::SetKeyDown(KeyboardButton key, bool value) {
    keyDown[static_cast<int>(key)] = value;
}

void Input::Update() {
    wasKeyDown = keyDown;
}

Input::InputKey::InputKey(KeyboardButton button)
    : button(button) {};

bool Input::InputKey::IsKeyDown() const
{
    return Engine::GetInput().keyDown[static_cast<int>(button)];;
}
bool Input::InputKey::IsKeyRelease() const
{
    return Engine::GetInput().keyDown[static_cast<int>(button)] == false && Engine::GetInput().wasKeyDown[static_cast<int>(button)] == true;
}
;

