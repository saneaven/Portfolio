/*
File name : Window.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Kevin Wright
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#include "doodle/window.hpp"        // doodle window stuff
#include "doodle/drawing.hpp"	// doodle clear background
#include "Engine.h"			// Engine.GetWindow()
#include "window.h"

void Window::Init(std::string windowName) {
	doodle::create_window(windowName);
	doodle::set_frame_of_reference(doodle::FrameOfReference::RightHanded_OriginBottomLeft);
	doodle::set_image_mode(doodle::RectMode::Corner);
}

void Window::Resize(int newWidth, int newHeight) { screenSize.x = newWidth; screenSize.y = newHeight; }

Vector2D<int> Window::GetSize() { return screenSize; }

void Window::Clear(Color color) { doodle::clear_background(doodle::HexColor{ color }); }

void Window::Update() { doodle::update_window(); }

void on_window_resized(int new_width, int new_height) { Engine::GetWindow().Resize(new_width, new_height); }
