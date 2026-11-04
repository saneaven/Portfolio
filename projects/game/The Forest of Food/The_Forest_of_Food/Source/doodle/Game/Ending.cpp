/*
File name : Ending.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Sieun Choi
Author(Second) : Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Ending.h"
#include "../Engine/Engine.h"
#include "../Engine/Rectangle.h"
#include "Screens.h"
#include "..\Engine\Sound.h"
#include "Inventory.h"

void GoodEnding::Load()
{
	maincharacter.SetFace(MainCharacterFace::HAPPY);
	maincharacter.SetPosition({ 0, 0 });
	fairy.SetFace(FairyFace::HAPPY);
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	ending = Engine::GetTextureManager().Load("assets/happyEnding.png");
	SetBGM("sounds/happyEnding.ogg");
	ConverationNum = 0;
}
void GoodEnding::Update(double )
{
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	Rectangle NextCheck = { {{Engine::GetWindow().GetSize().x / 15.0, Engine::GetWindow().GetSize().y / 15.0}
				,{Engine::GetWindow().GetSize().x / 15.0 + (Engine::GetWindow().GetSize().x / 15.0) * 13.0
				, Engine::GetWindow().GetSize().y / 15.0 + (Engine::GetWindow().GetSize().y / 15.0) * 4.0}}, {0, 0} };
	if (NextCheck.isPointInside({ MouseX, MouseY })) {
		if (mouse.MouseOneClick()) {
			ConverationNum++;
		}
	}
	if (next.IsKeyDown())
	{
		Engine::GetGameStateManager().SetNextState(MAINMENU);
	}
}

void GoodEnding::conversation() {
	double conversationSetX = (WindowX / 15) + 10;
	double conversationSetY = WindowY / 15.0 * 4.0;
	doodle::set_font(pixelFontStyle);
	switch (ConverationNum){
	case 0:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("You made really the same food!", conversationSetX, conversationSetY);		
		break;
	case 1:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("I want to leave you here! But I think you don't like it,\n so I'll send it home", conversationSetX, conversationSetY);
		break;		
	default:
		doodle::set_fill_color(doodle::Color{ 255 });
		maincharacter.Draw();
		fairy.Draw();
		doodle::set_font(pixelFontStyle);
		doodle::set_font_size(font_size);
		doodle::draw_text("HAPPY ENDING", Engine::GetWindow().GetSize().x / 4.0, Engine::GetWindow().GetSize().y / 2.0 + font_size * 2);
		doodle::draw_text("Finally YOU CAN GO HOME", Engine::GetWindow().GetSize().x / 4.0, Engine::GetWindow().GetSize().y / 2.0 + font_size);
		doodle::draw_text("Press R key for Restart", Engine::GetWindow().GetSize().x / 4.0, Engine::GetWindow().GetSize().y / 2.0);
		break;
	}

	doodle::pop_settings();
}

void GoodEnding::conversationWindow() {
	doodle::push_settings();
	doodle::set_outline_color(doodle::Color{ 0 });
	doodle::set_outline_width(1);
	doodle::set_fill_color(doodle::Color{ 255 , 230 });
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 15.0
		, (WindowX / 15.0) * 13.0, (WindowY / 15.0) * 4.0);
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 3.0
		, (WindowX / 15.0) * 2.0, (WindowY / 15.0) * 1.0);
	doodle::pop_settings();
	doodle::push_settings();
	doodle::set_font_size(32);
	doodle::set_fill_color(doodle::Color{ 0 });

}


void GoodEnding::UnLoad()
{
	ending = nullptr;
	PlayerName = "";
	UnloadBGM();
}

void GoodEnding::Draw()
{
	Engine::GetWindow().Clear(0xFFC0CBff);
	Vector2D<>pos = { static_cast<double>(Engine::GetWindow().GetSize().x / 4.0),static_cast<double>(Engine::GetWindow().GetSize().y / 4.0) };
	conversation();

}

void BadEnding::Load()
{
	maincharacter.SetFace(MainCharacterFace::DISAPPOINT);
	maincharacter.SetPosition({ 0, 0 });
	fairy.SetFace(FairyFace::DISAPPOINT);
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	ending = Engine::GetTextureManager().Load("assets/badEnding.png");
	SetBGM("sounds/badEnding.ogg");
	ConverationNum = 0;
}

void BadEnding::Draw()
{
	Engine::GetWindow().Clear(0x000000ff);
	conversation();
}

void BadEnding::conversation() {
	double conversationSetX = (WindowX / 15) + 10;
	double conversationSetY = WindowY / 15.0 * 4.0;
	doodle::set_font(pixelFontStyle);
	switch (ConverationNum) {
	case 0:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("You didn't make the same food that I gave you", conversationSetX, conversationSetY);
		break;
	case 1:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("I'm sorry, but you should stay here with me", conversationSetX, conversationSetY);
		break;
	default:
		doodle::set_fill_color(doodle::Color{ 255 });
		maincharacter.Draw();
		fairy.Draw();
		doodle::set_font_size(56.0);
		doodle::draw_text("Bad Ending", Engine::GetWindow().GetSize().x / 4.0, Engine::GetWindow().GetSize().y / 2.0 + font_size * 2);
		doodle::draw_text("REST IN THE FOREST OF FOOD", Engine::GetWindow().GetSize().x / 4.0, Engine::GetWindow().GetSize().y / 2.0 + font_size);
		doodle::draw_text("Press R key for Restart", Engine::GetWindow().GetSize().x / 4.0, Engine::GetWindow().GetSize().y / 2.0);
		break;
	}

	doodle::pop_settings();
}

void BadEnding::conversationWindow() {
	doodle::push_settings();
	doodle::set_outline_color(doodle::Color{ 0 });
	doodle::set_outline_width(1);
	doodle::set_fill_color(doodle::Color{ 255 , 230 });
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 15.0
		, (WindowX / 15.0) * 13.0, (WindowY / 15.0) * 4.0);
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 3.0
		, (WindowX / 15.0) * 2.0, (WindowY / 15.0) * 1.0);
	doodle::pop_settings();
	doodle::push_settings();
	doodle::set_font_size(32);
	doodle::set_fill_color(doodle::Color{ 0 });
}


void BadEnding::Update(double)
{
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	Rectangle NextCheck = { {{Engine::GetWindow().GetSize().x / 15.0, Engine::GetWindow().GetSize().y / 15.0}
				,{Engine::GetWindow().GetSize().x / 15.0 + (Engine::GetWindow().GetSize().x / 15.0) * 13.0
				, Engine::GetWindow().GetSize().y / 15.0 + (Engine::GetWindow().GetSize().y / 15.0) * 4.0}}, {0, 0} };
	if (NextCheck.isPointInside({ MouseX, MouseY })) {
		if (mouse.MouseOneClick()) {
			ConverationNum++;
		}
	}
	if (next.IsKeyDown())
	{
		Engine::GetGameStateManager().SetNextState(MAINMENU);
	}
}
void BadEnding::UnLoad()
{
	ending = nullptr;
	PlayerName = "";
	UnloadBGM();
}


void HiddenEnding::Load(){
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	ending = Engine::GetTextureManager().Load("assets/hiddenEnding.png");
	hiddenEnding = Engine::GetTextureManager().Load("assets/HiddenEndingWithCharacter.png");
	SetBGM("sounds/hiddenEnding.ogg");
	ConverationNum = 0;
}
	
void HiddenEnding::Draw() {
	Engine::GetWindow().Clear(0xf2c7d9ff);
	conversation();
}

void HiddenEnding::UnLoad() {
	ending = nullptr;
	hiddenEnding = nullptr;
	UnloadBGM();
}

void HiddenEnding::Update(double) {
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	Rectangle NextCheck = { {{Engine::GetWindow().GetSize().x / 15.0, Engine::GetWindow().GetSize().y / 15.0}
				,{Engine::GetWindow().GetSize().x / 15.0 + (Engine::GetWindow().GetSize().x / 15.0) * 13.0
				, Engine::GetWindow().GetSize().y / 15.0 + (Engine::GetWindow().GetSize().y / 15.0) * 4.0}}, {0, 0} };
	if (NextCheck.isPointInside({ MouseX, MouseY })) {
		if (mouse.MouseOneClick()) {
			ConverationNum++;
		}
	}
	if (next.IsKeyDown())
	{
		Engine::GetGameStateManager().SetNextState(MAINMENU);
	}
}

void HiddenEnding::conversation(){
	double conversationSetX = (WindowX / 15) + 10;
	double conversationSetY = WindowY / 15.0 * 4.0;
	doodle::set_font(pixelFontStyle);
	switch (ConverationNum) {
	case 0:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("What taste is this?!!", conversationSetX, conversationSetY);
		break;
	case 1:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("Is this the taste of MSG I've only heard in words?", conversationSetX, conversationSetY);
		break;
	case 2:
		ending->Draw();
		conversationWindow();
		fairy.DrawFairyName();
		doodle::draw_text("I was impressed by you. This is my gift.\nPlease stay here and live with me.", conversationSetX, conversationSetY);
		break;
	default:
		Engine::GetWindow().Clear(0xffffffff);
		hiddenEnding->Draw();
		break;
	}

	doodle::pop_settings();
}

void HiddenEnding::conversationWindow() {
	doodle::push_settings();
	doodle::set_outline_color(doodle::Color{ 0 });
	doodle::set_outline_width(1);
	doodle::set_fill_color(doodle::Color{ 255 , 230 });
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 15.0
		, (WindowX / 15.0) * 13.0, (WindowY / 15.0) * 4.0);
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 3.0
		, (WindowX / 15.0) * 2.0, (WindowY / 15.0) * 1.0);
	doodle::pop_settings();
	doodle::push_settings();
	doodle::set_font_size(32);
	doodle::set_fill_color(doodle::Color{ 0 });
}
