/*
	File name: main.cpp
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	primary author: Minjae Kyung
	secondary author: Haeun Park
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#include <doodle/doodle.hpp>

#include <iostream>
#include <vector>

#include "classes.h"
#include "basicFunc.h"

using namespace std;
using namespace doodle;

void createMap(Player& player, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items);
void credits_screen(float x, float y);

Player player(-600, -80, 20, 40, 1);
Camera camera(&player, 1.3);

Button start_button("Start", 0, 0, 280, 80);
Button credits_button("Credits", 0, -110, 280, 80);
Button quit_button("Quit", 0, -220, 280, 80, 45);

char GameState = 'L';
unsigned int BallMode = 0;

int main(void)
{
	vector<Enemy> enemies;
	vector<Block> blocks;
	vector<Bullet> bullets;
	bool isBulletHits;
	vector<Item> items;

	int alpha_logo = 255;

	create_window();

	float credits_y = -float(Width) / 2 - 30;
	Button titleScreen_button("back", float(Width) / 2, -float(Height) / 2, 120, 50);

	createMap(player, enemies, blocks, items);

	// create textures
	Texture        digipen_logo;
	constexpr auto texture_path = "DigiPen_BLACK_1024px.png";
	const bool loaded = digipen_logo.LoadFromPNG(texture_path);
	Texture        digipen_copyright;
	constexpr auto texture_path2 = "Digipen_Copyright.png";
	const bool loaded2 = digipen_copyright.LoadFromPNG(texture_path2);
	if (loaded || loaded2)
	{
		//return -1;
	}

	set_texture_mode(RectMode::Center);
	set_rectangle_mode(RectMode::Center);

	cout << "Hello doodle!\n";

	toggle_full_screen();
	while (!is_window_closed())
	{
		set_window_title("Prototype 1");
		update_window();
		clear_background(HexColor{ 0xFFFFFFFF });
		switch (GameState)
		{
		case 'L': // logo screen
			// draw digipen logo
			draw_texture(digipen_logo, 0, 0);

			// fade in - fade out
			set_tint_color(HexColor{ alpha_logo });
			if (ElapsedTime > 2 && alpha_logo > 1) { alpha_logo -= 2; }
			if (alpha_logo == 1) { GameState = 'T'; }
			break;

		case 'T': // title screen
			push_settings();
			doodle::set_font_size(113);
			set_font_backdrop_offset(3, 3);
			draw_text("Painter", -240, 100);
			pop_settings();
			set_texture_mode(RectMode::Corner);
			no_tint();
			draw_texture(digipen_copyright, -float(Width) / 2, -float(Height) / 2, 550, 49);
			start_button.draw();
			credits_button.draw();
			quit_button.draw();
			if (start_button.isMouseOnButton() && MouseIsPressed) { GameState = 'R'; }
			if (credits_button.isMouseOnButton() && MouseIsPressed) { GameState = 'C'; }
			if (quit_button.isMouseOnButton() && MouseIsPressed) { close_window(); }

			break;

		case 'R'://Reload Game
			enemies.clear();
			blocks.clear();
			bullets.clear();
			items.clear();
			createMap(player, enemies, blocks, items);

			GameState = 'G';

			break;

		case 'G': // game start

			if (!player.isDead)
				player.update(bullets, enemies, blocks, items);
			if (player.isStageClear)
				GameState = 'C';


			for (Block& i : blocks)
				i.draw(camera);

			for (Enemy& i : enemies)
			{
				i.update(blocks);
				i.draw(camera);
			}

			for (int i = 0; i < bullets.size(); i++)
			{
				isBulletHits = bullets[i].update(enemies, blocks);
				bullets[i].draw(camera);

				if (isBulletHits)
				{
					bullets.erase(bullets.begin() + i);
					--i;
					isBulletHits = false;
				}
			}

			push_settings();
			set_fill_color(HexColor{ unsigned int(player.getBallColor()) });
			set_font_size(30);
			no_outline();
			draw_text("Num of paint : " + to_string(player.getBallStorage()), float(Width) / 2 - 400, float(Height) / 2 - 50);
			pop_settings();

			if (player.getPosition().yValue() < -Width / 2 || player.isDead)
			{
				set_font_size(50);
				draw_text("press 'R' key to restart the game", -500, float(Height) / 2 - 250);
			}


			for (Item& i : items)
				i.draw(camera);

			if (!player.isDead)
			{
				player.draw(camera);
				player.drawBall(camera);
			}



			camera.update();
			break;

		case 'C': //credits screen
			credits_screen(0, credits_y);
			credits_y += 3;

			titleScreen_button.draw();
			if (titleScreen_button.isMouseOnButton() && MouseIsPressed) { player.isStageClear = false; GameState = 'T'; credits_y = -float(Width) / 2 - 30; }
			break;
		}
	}

	return 0;
}


void on_key_pressed(KeyboardButtons button)
{
	switch (button)
	{
	case KeyboardButtons::Right:
		player.isRight = true;
		player.staringDirection = Direction::right;
		break;
	case KeyboardButtons::Left:
		player.isLeft = true;
		player.staringDirection = Direction::left;
		break;
	case KeyboardButtons::Up:
		if (player.isOnGround)
			player.isJump = true;
		break;
	case KeyboardButtons::Space:
		player.isShoot = true;
		break;
	case KeyboardButtons::R:
		GameState = 'R';
		break;
	case KeyboardButtons::Z:
		BallMode++;
		switch (BallMode % 3)
		{
		case 0:
			player.setBallColor(0xFF0000FF);
			break;
		case 1:
			player.setBallColor(0x00FF00FF);
			break;
		case 2:
			player.setBallColor(0x0000FFFF);
			break;
		}
		break;
	}
}
void on_key_released(KeyboardButtons button)
{
	switch (button)
	{
	case KeyboardButtons::Right:
		player.isRight = false;
		break;
	case KeyboardButtons::Left:
		player.isLeft = false;
		break;
	}
}

void credits_screen(float x, float y)
{
	set_font_size(35);

	if (player.isStageClear)
	{
		draw_text("Congratuations!", x - 145, y + 100);
	}
		

	draw_text("<President>", x - 115, y);
	y -= 60;
	draw_text("Claude Comair", x - 135, y);
	y -= 100;

	draw_text("<Instructor>", x - 125, y);
	y -= 60;
	draw_text("David Ly", x - 80, y);
	y -= 60;
	draw_text("Rudy Castan", x - 120, y);
	y -= 100;

	draw_text("<Lead Programmer>", x - 210, y);
	y -= 60;
	draw_text("Minjae Kyung", x - 130, y);
	y -= 100;

	draw_text("<Sub Programmer>", x - 200, y);
	y -= 60;
	draw_text("Haeun Park", x - 120, y);
	y -= 100;

	draw_text("<Game Designer>", x - 180, y);
	y -= 60;
	draw_text("Minjae Kyung", x - 130, y);
	y -= 60;
	draw_text("Haeun Park", x - 120, y);

}