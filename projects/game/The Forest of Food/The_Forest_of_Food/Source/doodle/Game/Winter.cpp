/*
File name : Winter.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) :Sieun Choi
Author(Second):Hyeonseo Yoon
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Winter.h"
#include "Screens.h"


Winter::Winter(){}

void Winter::Load() 
{
	std::string path = "Map_Design/Winter.png";
	Engine::GetMap().Load(path, WINTER);
	objectManager.Load(Engine::GetMap().GetSeason());
	interface.Load();
	SetBGM("sounds/winter.ogg");
	WindowX = Engine::GetWindow().GetSize().x;
	WindowY = Engine::GetWindow().GetSize().y;
	MouseX = doodle::get_mouse_x();
	MouseY = doodle::get_mouse_y();
	gameStart = false;
}
void Winter::Update(double dt)
{
	if (gameStart == false) {
		WindowX = Engine::GetWindow().GetSize().x;
		WindowY = Engine::GetWindow().GetSize().y;
		MouseX = doodle::get_mouse_x();
		MouseY = doodle::get_mouse_y();
		if (NextCheck.isPointInside({ MouseX, MouseY })) {
			if (mouseClick.MouseOneClick()) {
				ConverationNum++;
			}
		}
	}
	else{
		objectManager.UpdateAll(dt);

		///////////////////////////
		if (SetSpring.IsKeyRelease()) {
			InventoryItems.clear();
			Engine::GetGameStateManager().SetNextState(SPRING);
		}
		else if (SetSummer.IsKeyRelease()) {
			InventoryItems.clear();
			Engine::GetGameStateManager().SetNextState(SUMMER);
		}
		else if (SetFall.IsKeyRelease()) {
			InventoryItems.clear();
			Engine::GetGameStateManager().SetNextState(FALL);
		}
		else if (SetWinter.IsKeyRelease()) {
			InventoryItems.clear();

			Engine::GetGameStateManager().SetNextState(WINTER);
		}
		///////////////////////////
		interface.Update();
	}
	
	interface.CameraUpdate(objectManager);
	
}

void Winter::UnLoad()
{
	objectManager.Unload();
	UnloadBGM();
	interface.UnLoad();
}



void Winter::Draw()
{
	doodle::clear_background(doodle::HexColor{ 0x000000ff });
	Engine::GetMap().Draw(interface.GetCameraMatrix());
	objectManager.DrawAll(interface.GetCameraMatrix(), Engine::GetMap().GetSeason());
	interface.Draw();
	if (gameStart == false) {
		faity.Draw();
		mainCharacter.Draw();
		mainCharacter.SetFace(MainCharacterFace::DISAPPOINT);
		conversation();
	}
}



void Winter::conversation() {
	double conversationSetX = (WindowX / 15) + 10;
	double conversationSetY = WindowY / 15.0 * 4.0;
	doodle::push_settings();
	doodle::set_outline_color(doodle::Color{ 0 });
	doodle::set_outline_width(1);
	doodle::set_fill_color(doodle::Color{ 255 , 230 });
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 15.0
		, (WindowX / 15.0) * 13.0, (WindowY / 15.0) * 4.0);
	doodle::draw_rectangle(WindowX / 15.0, WindowY / 3.0
		, (WindowX / 15.0) * 2.0, (WindowY / 15.0) * 1.0);
	doodle::pop_settings();
	doodle::set_font(pixelFontStyle);
	doodle::push_settings();
	doodle::set_font_size(32);
	doodle::set_fill_color(doodle::Color{ 0 });
	switch (ConverationNum)
	{
	case 0:
		doodle::draw_text("Welcome. The Forest of Food!", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 1:
		doodle::draw_text("You want to go home? I'll send you home if you just bring the ingredients.", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::SOSO);
		faity.DrawFairyName();
		break;
	case 2:
		doodle::draw_text("After you've gathered the Jeongol ingredients you just ate, \nyou can click the pot in the top right corner and move them to where \nyou can pick them.", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 3:
		doodle::draw_text("If you collect the ingredients of Jeongol and\n cook them exactly like the food I gave you, I will send you home!", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::SOSO);
		faity.DrawFairyName();
		break;
	case 4:
		doodle::draw_text("If you fail, you will have to live with me here!", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	case 5:
		doodle::draw_text("I'll give you 5 minutes to collect the ingredients! Watch the timer on top. \nAfter 5 minutes, you can't collect any more materials. Good luck!", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::SOSO);
		faity.DrawFairyName();
		break;
	default:
		gameStart = true;
		doodle::draw_text("I'll give you 5 minutes to collect the ingredients! Watch the timer on top. \nAfter 5 minutes, you can't collect any more materials. Good luck!", conversationSetX, conversationSetY);
		faity.SetFace(FairyFace::HAPPY);
		faity.DrawFairyName();
		break;
	}
	doodle::pop_settings();
}