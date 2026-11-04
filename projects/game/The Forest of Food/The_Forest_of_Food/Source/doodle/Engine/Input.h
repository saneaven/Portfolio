/*
File name : Input.h
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <vector>

class Input {
public:
	enum class KeyboardButton {
		None, Enter, Escape, Space, Left, Up, Right, Down
		, Backspace, CapsLock, Control, Shift,
		A, B, C, D, E, F, G, H, I, J,
		K, L, M, N, O, P, Q, R, S, T,
		U, V, W, X, Y, Z,
		Count
	};
	class InputKey {

	public:

		InputKey(KeyboardButton button);
		bool IsKeyDown() const;
		bool IsKeyRelease() const;
	private:
		KeyboardButton button;



	};
	Input();
	bool IsKeyDown(KeyboardButton key) const;
	bool IsKeyUp(KeyboardButton key) const;
	bool IsKeyReleased(KeyboardButton key) const;
	void SetKeyDown(KeyboardButton key, bool value);
	void Update();
private:
	std::vector<bool> keyDown;
	std::vector<bool> wasKeyDown;

};

