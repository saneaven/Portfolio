/*
File name : doodle.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Haneul Lee, Hoseob Jeong
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <doodle/doodle.hpp>
#include <random>
#include <ctime>
#include "Map.h"
#include "basicFunc.h"
#include "classes.h"
#include "SFX.h"
using namespace doodle;
using namespace std;
GAME Game;
Player player(1*BLOCKSIZE, -6*BLOCKSIZE, 25, 40, 1);
Camera camera(&player, 1.3);
std::vector<Sonar> sonars;
std::vector<GateSonar> gateSonars;

Map CurrentMap;
Gate gate;

float bgm_volume = 50;

void mainGame(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items);


int main(void)
{
	srand((int)time(NULL));
	create_window();

	load_sound();

	sf::Music music;
	if (!music.openFromFile("sfx/main_bgm.ogg"))
	{
		cout << "Failed to load the music file" << endl;
	}
	if (music.getStatus() != sf::SoundSource::Playing)
	{
		music.setVolume(bgm_volume);
		music.setLoop(true);
		music.play();
	}

	Game.DigiPen_screen();
	vector<Enemy> enemies;
	vector<Block> blocks;
	vector<Bullet> bullets;
	vector<Item> items;

	CurrentMap = ReadMap(mapFilePath);
	setup_Map(CurrentMap, blocks, enemies, player, gate);

	vector<Syllable> gate_key_notes = { Syllable::Do, Syllable::Mi, Syllable::La };
	gate.setKey(gate_key_notes);

	

	initHUD();
	set_texture_mode(RectMode::Center);
	set_rectangle_mode(RectMode::Center);

	cout << "Hello David!\n";
	bool isPlaying = false;

	while (!is_window_closed())
	{
		set_window_title("Sonar SOuL");
		update_window();
		clear_background(HexColor{ 0x77CCFFFF });
				
		animationUpdate();
		timerUpdate();

		switch (Game.CurrentScreen) {
		case Screen::Main_menu:
			Game.main_menu();
			break;
		case Screen::PlayGame:
			mainGame(bullets, enemies, blocks, items);
			break;
		case Screen::Credit:
			Game.credit();
			break;
		case Screen::Reload:
			bullets.clear();
			items.clear();
			if (!isPlaying)
			{
				player.moveAnimation(player.getPosition(), player.spawnPoint, 1000);
				isPlaying = true;
			}

			mainGame(bullets, enemies, blocks, items);
			setTimer([&blocks, &enemies, &isPlaying]()
				{
					isPlaying = false;
					enemies.clear();
					blocks.clear();
					setup_Map(CurrentMap, blocks, enemies, player, gate);
					vector<Syllable> gate_key_notes = { Syllable::Do, Syllable::Mi, Syllable::La };
					gate.setKey(gate_key_notes);
					Game.CurrentScreen = Screen::PlayGame;
				}, 1100);
			break;
		case Screen::Ending:
			if (!isPlaying)
			{
				bgm_volume = 0;
				gate.play_gate_song();
				isPlaying = true;
			}
			Game.CurrentScreen = Screen::Credit;

			break;
		}

	}
    return 0;
}

void mainGame(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items)
{
	

	bool isBulletHits = false;
	player.relativeMouseUpdate(camera);

	if (DeltaTime > 0.03f)
		DeltaTime = 0.03f;


	for (Block& i : blocks)
		i.draw(camera);

	for (Enemy& i : enemies)
	{
		i.update(blocks);
		i.draw(camera);
	}

	if (!player.isDead)
		player.update(bullets, enemies, blocks, items);

	for (int i = 0; i < bullets.size(); i++)
	{
		isBulletHits = bullets[i].update(enemies, blocks, items, gate);
		bullets[i].draw(camera);
		if (isBulletHits)
		{
			bullets.erase(bullets.begin() + i);
			--i;
			isBulletHits = false;
		}
	}

	for (Item i : items)
	{
		i.draw(camera);
	}

	for (int i = 0; i < sonars.size(); i++)
	{
		if (!sonars[i].update(enemies))
		{
			sonars.erase(sonars.begin() + i);
			--i;
			continue;
		}
		sonars[i].draw(camera);
	}

	for (int i = 0; i < gateSonars.size(); i++)
	{
		if (!gateSonars[i].update(gate))
		{
			gateSonars.erase(gateSonars.begin() + i);
			--i;
			continue;
		}
		gateSonars[i].draw(camera);
	}


	camera.update();
	if (!player.isDead)
	{
		player.draw(camera);
	}

	gate.draw(camera);

	updateConstHUD();
	UpdateHUD(player);
	drawHUD(player);

	make_cursor();
}

void on_mouse_pressed(MouseButtons button)
{
	if (button == MouseButtons::Left)
	{
		player.isShoot = true;
	}

}

bool is_space_pressed = false;

void on_key_pressed(KeyboardButtons button)
{
	if (is_space_pressed) {
		switch (button)
		{
		case KeyboardButtons::Q:
			player.use_note(player.q_note, Syllable::Sol);
			break;
		case KeyboardButtons::W:
			player.use_note(player.w_note, Syllable::La);
			break;
		case KeyboardButtons::E:
			player.use_note(player.e_note, Syllable::Ti);
			break;
		case KeyboardButtons::R:
			player.use_note(player.r_note, Syllable::Do_high);
			break;
		case KeyboardButtons::A:
			player.use_note(player.a_note, Syllable::Do);
			break;
		case KeyboardButtons::S:
			player.use_note(player.s_note, Syllable::Re);
			break;
		case KeyboardButtons::D:
			player.use_note(player.d_note, Syllable::Mi);
			break;
		case KeyboardButtons::F:
			player.use_note(player.f_note, Syllable::Fa);
			break;
		case KeyboardButtons::L:
			Game.CurrentScreen = Reload;
			break;
		}
	}
	else 
	{
		switch (button)
		{
		case KeyboardButtons::D:
			if (player.d_note) 
			{
				player.isRight = true;
				player.staringDirection = Direction::right;
			}
			break;
		case KeyboardButtons::A:
			if (player.a_note)
			{
				player.isLeft = true;
				player.staringDirection = Direction::left;
			}
			break;
		case KeyboardButtons::W:
			if (player.w_note)
			{
				if (player.isOnGround)
					player.isJump = true;
			}
			break;
		case KeyboardButtons::S:
			if (player.s_note)
			{
				player.crouch(true);
			}
			break;
		case KeyboardButtons::R:
			if (player.r_note)
			{
				player.reloadBullet();
			}
			break;
		case KeyboardButtons::Q:
			if (player.q_note)
			{
				--GateNoteNum;
			}
			break;

		case KeyboardButtons::F:
			if (player.f_note)
			{
				player.use_gateSonar(gateSonars, (Syllable)GateNoteNum);
			}
			break;
		case KeyboardButtons::E:
			if (player.e_note)
			{
				++GateNoteNum;
			}
			break;
		case KeyboardButtons::Space:
			is_space_pressed = true;
			for (int i = 0; i < 8; i++)
				HUDs[i].moveAnimation(
					Vec2(notePosX + noteSize * ((7 - i) % 4), notePosY + noteSize * (7 - i) / 4),
					Vec2(notePosX, notePosY - noteSize * i + doodle::Height / 10),
					200);

			break;

		case KeyboardButtons::L:
			Game.CurrentScreen = Reload;
			break;
		}
	}
}
void on_key_released(KeyboardButtons button)
{
	switch (button)
	{
	case KeyboardButtons::D:
		if (player.d_note)
		{
			player.isRight = false;
		}
		break;
	case KeyboardButtons::A:
		if (player.a_note)
		{
			player.isLeft = false;
		}
		break;
	case KeyboardButtons::S:
		if (player.s_note)
		{
			player.crouch(false);
		}
		break;
	case KeyboardButtons::Space:
		is_space_pressed = false;
		for (int i = 0; i < 8; i++)
			HUDs[i].moveAnimation(Vec2(notePosX, notePosY - noteSize * i + doodle::Height / 10),
								Vec2(notePosX + noteSize * ((7 - i) % 4), notePosY + noteSize * (7 - i) / 4),
								200);


		break;
	case KeyboardButtons::O:
		toggle_full_screen();
		break;
	case KeyboardButtons::Escape:
		close_window();
		break;
	}
}