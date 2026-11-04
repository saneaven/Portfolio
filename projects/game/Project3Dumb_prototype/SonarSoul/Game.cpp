/*
File name : Game.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author :  Hoseob Jeong
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include "classes.h"
#include "doodle/doodle.hpp"



using namespace doodle;
void GAME::main_menu() {
	
	draw_text("SONAR SOUL", -200.0f, 200.0f);

	//
	push_settings();
	set_rectangle_mode(RectMode::Center);
	set_font_size(20);
	draw_rectangle(0, PLAY_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	draw_text("PLAY", PLAY_BUTTON_X +BUTTON_WIDTH/5.0f, PLAY_BUTTON_Y-BUTTON_HEIGHT/2.0f);
	
	draw_rectangle(0, CREDIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	draw_text("CREDIT", CREDIT_BUTTON_X, CREDIT_BUTTON_Y - BUTTON_HEIGHT/2.0f);
	
	draw_text("All content (C) 2019 DigiPen(USA) Corporation, all rights reserved.", -400.0f, -300.0f);
	pop_settings();
	isMouseClicked();
	if (isClicked && mouse_interact( PLAY_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
		std::cout << "play clicked" << std::endl;
		CurrentScreen = Screen::PlayGame;
		isClicked = false;
	}
	if (isClicked && mouse_interact( CREDIT_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
		CurrentScreen = Screen::Credit;
		std::cout << "credit clicked" << std::endl;
		isClicked = false;
	}
	isClicked = false;
}

void GAME::credit() {
	draw_text("CREDIT", -150.0f,200.0f);
	push_settings();
	set_rectangle_mode(RectMode::Center);
	set_font_size(15);
	draw_text("President: Claude Comair", -200.0f, CREDIT_START_Y - CREDIT_RANGE);
	draw_text("Instructor: David.ly", -200.0f, CREDIT_START_Y - 2*CREDIT_RANGE);
	draw_text("Team 3D", -80.0f, CREDIT_START_Y - 3 * CREDIT_RANGE);
	draw_text("Dumb(Main programmer & designer) = minjae.kyung", -350.0f, CREDIT_START_Y - 4*CREDIT_RANGE);
	draw_text("Dumber(Sub programmer & designer) = hoseob.jeong", -350.0f, CREDIT_START_Y - 5 * CREDIT_RANGE);
	draw_text("Dumbest(Sub programmer & designer) = haneul.lee", -350.0f, CREDIT_START_Y - 6 * CREDIT_RANGE);
	draw_text("-Copyright-", -80.0f, CREDIT_START_Y - 9 * CREDIT_RANGE);
	draw_text("-shoot.ogg produced by MusicLegends. And reproduced by Haneul Lee.", -350.0f, CREDIT_START_Y -10 * CREDIT_RANGE);
	draw_text("-main_bgm.ogg produced by KiluaBoy.And reproduced by Haneul Lee.", -350.0f, CREDIT_START_Y -11 * CREDIT_RANGE);
	draw_text("-end_bgm.ogg produced by contactamoghsymphonymusic.And reproduced by Haneul Lee.", -350.0f, CREDIT_START_Y -12 * CREDIT_RANGE);
	draw_rectangle(0, BACK_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT);
	draw_text("BACK", BACK_BUTTON_X + BUTTON_WIDTH/5.0f, BACK_BUTTON_Y- BUTTON_HEIGHT/2.0f);
	pop_settings();
	
	

	isMouseClicked();
	if (isClicked && mouse_interact( BACK_BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT)) {
		CurrentScreen = Screen::Main_menu;
		isClicked = false;
	}
	isClicked = false;
}

bool GAME::mouse_interact( const float&button_y, const float&width, const float&height)
{
	float mouseX = static_cast<float>(get_mouse_x());
	float mouseY = static_cast<float>(get_mouse_y());

	if (((-width/2.0f < mouseX )&& (mouseX < width/2.0f)) && ((button_y - height/2.0f < mouseY) && (mouseY < button_y+ height/2.0f) ))
	{

		return true;
	}
	return false;
}

void GAME::DigiPen_screen() {
	Texture        DigipenSplash;
	DigipenSplash.LoadFromPNG("DigiPen_BLACK_1024px.png");


	int SplashFrame = 180;

	while (!is_window_closed() && SplashFrame > 0)
	{
		update_window();
		clear_background(HexColor{ 0xFFFFFFFF });
		push_settings();
		set_texture_mode(RectMode::Center);
		draw_texture(DigipenSplash, 0, 0, 1024, 246);
		pop_settings();
		SplashFrame--;
	}

}
void GAME::isMouseClicked() {
	if (MouseIsPressed) {

		isClicked = true;
	}
	else {
		isClicked = false;
	
	}
}


