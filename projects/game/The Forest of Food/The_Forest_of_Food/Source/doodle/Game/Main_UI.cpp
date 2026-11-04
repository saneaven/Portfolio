/*
File name : Main_UI.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Sieun Choi
Author(Second):MinJae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Main_UI.h"
#include "../Engine/Engine.h"
#include "doodle/drawing.hpp"
#include "Screens.h"
using namespace doodle;

MainMenu::MainMenu() :window_s(Engine::GetWindow().GetSize()), setstate(NULL),ChoiceBox(nullptr),Colored_Box(nullptr),Main(nullptr),Point(nullptr)
,animationValue(0),animationValue2(0),MainFullScreen(nullptr),Full(false){
}

void MainMenu::Load()
{
	SetBGM("sounds/mainMenu.ogg");
	Engine::GetInterpolationManager().AddTimeLerp(&animationValue, -5000, 0, 5, 0, 0.8); // example of animation
	animationValue2 = 200;
	ChoiceBox = Engine::GetTextureManager().Load("assets/choicebox.png");
	Colored_Box = Engine::GetTextureManager().Load("assets/c_choicebox.png");
	Main = Engine::GetTextureManager().Load("assets/MainMenu.png");
	MainFullScreen = Engine::GetTextureManager().Load("assets/MainFull.png");
	Point = Engine::GetTextureManager().Load("assets/Pointer.png");
	BoxSize = { 400,100 };
	Full = false;
}
void MainMenu::Draw()
{
	push_settings();
	doodle::set_font(font_style);
	if (!Full)
		Main->Draw(TranslateMatrix(Vector2D<>{ (Engine::GetWindow().GetSize().x - static_cast<__int64>(Main->GetSize().x)) / 2.0, (Engine::GetWindow().GetSize().y - static_cast<__int64>(Main->GetSize().y)) / 2.0 }));
	else if (Full)
		MainFullScreen->Draw({ 0, 0 });
	if (animationValue >= -500 && animationValue2 == 200) 
	{
		Engine::GetInterpolationManager().AddTimeLerp(&animationValue2, 200, 0, 3, 1, 1); // example of animation
	}

	doodle::set_font_size(36);
	set_fill_color(HexColor{ 0x3a9ef3ff });
	smooth_drawing();
	Box(Vector2D<>{Engine::GetWindow().GetSize().x / 4.0 + animationValue2, Engine::GetWindow().GetSize().y / 2.0 - BoxSize.y }, ChoiceBox->GetSize(), STORY);
	Box(Vector2D<>{Engine::GetWindow().GetSize().x / 2.0 - BoxSize.x/2, Engine::GetWindow().GetSize().y / 2.0 - BoxSize.y + animationValue }, ChoiceBox->GetSize(), TUTORIAL);
	Box(Vector2D<>{Engine::GetWindow().GetSize().x - static_cast<double>(Engine::GetWindow().GetSize().x) / 3.0 - BoxSize.x/3.0 - animationValue2 - 15.0, Engine::GetWindow().GetSize().y / 2.0 - BoxSize.y }, ChoiceBox->GetSize(), QUIT);
	doodle::draw_text("Play Game", Engine::GetWindow().GetSize().x / 4.0 + 35.0 + animationValue2, Engine::GetWindow().GetSize().y / 2.0 - BoxSize.y + 25.0);
	doodle::draw_text("How To Play", Engine::GetWindow().GetSize().x / 2.0 - BoxSize.x / 3 - 5.0, Engine::GetWindow().GetSize().y / 2.0 - BoxSize.y + 25.0 + animationValue);
	doodle::draw_text("Quit", static_cast<double>(Engine::GetWindow().GetSize().x) - static_cast<double>(Engine::GetWindow().GetSize().x) / 3 - BoxSize.x/3 + 70.0 - animationValue2, Engine::GetWindow().GetSize().y / 2.0 - BoxSize.y + 25);
	Credit();
	pop_settings();

}
void MainMenu::Update(double)
{
	Engine::GetWindow().Clear(0x6ab6f6ff);
	
	if (window_s != Engine::GetWindow().GetSize())
	{
		Full = true;
	}
	else if (window_s == Engine::GetWindow().GetSize())
	{
		Full = false;
	}
}

void MainMenu::UnLoad()
{
	ChoiceBox = nullptr;
	Colored_Box = nullptr;
	Main = nullptr;
	MainFullScreen = nullptr;
	Point = nullptr;
}

void MainMenu::Box(Vector2D<>pos, Vector2D<> size, int state)
{
	Vector2D<> Mouse = { static_cast<double>(doodle::get_mouse_x()),static_cast<double>(doodle::get_mouse_y()) };

	if (Mouse.x > pos.x && Mouse.x < pos.x + size.x && Mouse.y <= pos.y + size.y && Mouse.y >= pos.y)
	{
		Colored_Box->Draw(pos);
		if (mouseClick.MouseOneClick())
		{
			if (state == QUIT) {
				Engine::GetGameStateManager().Shutdown();
				doodle::close_window();
			}
			else {
				if (state == STORY)
				{
					UnloadBGM();
					Engine::GetGameStateManager().SetNextState(state);
				}
				else if(state == TUTORIAL){
					Engine::GetGameStateManager().SetNextState(state);
				}
				
				
			}

		}
	}
	else {

		ChoiceBox->Draw(pos);
	}


}

void MainMenu::Credit()
{
	Vector2D<> pos = { static_cast<double>(Engine::GetWindow().GetSize().x) - BoxSize.x,BoxSize.y/3 };
	BoxSize = { 203,60 };
	Vector2D<> Mouse = { static_cast<double>(doodle::get_mouse_x()),static_cast<double>(doodle::get_mouse_y()) };

	if (Mouse.x > pos.x&& Mouse.x < pos.x + BoxSize.x && Mouse.y <= pos.y + BoxSize.y && Mouse.y >= pos.y)
	{
		pos = { pos.x + BoxSize.x/2 + Point->GetSize().x,BoxSize.y/3 };
		Point->Draw(pos);
		if (mouseClick.MouseOneClick())
		{
			Engine::GetGameStateManager().SetNextState(CREDITSCREEN);

		}
	}

}


