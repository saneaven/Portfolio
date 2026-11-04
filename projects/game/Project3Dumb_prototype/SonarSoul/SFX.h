/*
File name : SFX.h
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : Haneul Lee
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#pragma once
#include <vector>
#include <SFML/Audio.hpp>
#include <iostream>

extern inline std::vector <sf::SoundBuffer> P_soundBuffers{}; //Player's buffer
extern inline std::vector <sf::SoundBuffer> E_soundBuffers{}; //Enemy's buffer
extern inline std::vector <sf::SoundBuffer> G_soundBuffers{}; //Gate's buffer
extern inline std::vector <sf::SoundBuffer> Shoot_Buffers{}; //Shoot's buffer
extern inline std::vector <sf::SoundBuffer> BGM_Buffers{}; //BGM's buffer

extern inline std::vector <sf::Sound> P_sounds{};
extern inline std::vector <sf::Sound> E_sounds{};
extern inline std::vector <sf::Sound> G_sounds{};
extern inline std::vector <sf::Sound> Shoot_sounds{};
extern inline std::vector <sf::Sound> BGM_sounds{};

void P_LoadSound(const std::string& file_path);
void E_LoadSound(const std::string& file_path);
void G_LoadSound(const std::string& file_path);
void Shoot_LoadSound(const std::string& file_path);
void BGM_LoadSound(const std::string& file_path);
void load_sound();