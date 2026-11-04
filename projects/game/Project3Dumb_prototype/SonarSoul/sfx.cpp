/*
File name : sfx.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : Haneul Lee
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include "SFX.h"

void P_LoadSound(const std::string& file_path)
{
	P_soundBuffers.emplace_back();
	sf::SoundBuffer& buffer = P_soundBuffers.back();
	if (!buffer.loadFromFile(file_path))
	{
		std::cout <<"Failed to load" << file_path << "\n";
	}
}

void E_LoadSound(const std::string& file_path)
{
	E_soundBuffers.emplace_back();
	sf::SoundBuffer& buffer = E_soundBuffers.back();
	if (!buffer.loadFromFile(file_path))
	{
		std::cout << "Failed to load" << file_path << "\n";
	}
}

void G_LoadSound(const std::string& file_path)
{
	G_soundBuffers.emplace_back();
	sf::SoundBuffer& buffer = G_soundBuffers.back();
	if (!buffer.loadFromFile(file_path))
	{
		std::cout << "Failed to load" << file_path << "\n";
	}
}

void Shoot_LoadSound(const std::string& file_path)
{
	Shoot_Buffers.emplace_back();
	sf::SoundBuffer& buffer = Shoot_Buffers.back();
	if (!buffer.loadFromFile(file_path))
	{
		std::cout << "Failed to load" << file_path << "\n";
	}
}

void BGM_LoadSound(const std::string& file_path)
{
	BGM_Buffers.emplace_back();
	sf::SoundBuffer& buffer = BGM_Buffers.back();
	if (!buffer.loadFromFile(file_path))
	{
		std::cout << "Failed to load" << file_path << "\n";
	}
}

void load_sound() 
{
	//Player sounds load
	P_LoadSound("sfx/Lush_-_Do_low.ogg");
	P_LoadSound("sfx/Lush_-_Re.ogg");
	P_LoadSound("sfx/Lush_-_Mi.ogg");
	P_LoadSound("sfx/Lush_-_Pa.ogg");
	P_LoadSound("sfx/Lush_-_Sol.ogg");
	P_LoadSound("sfx/Lush_-_Ra.ogg");
	P_LoadSound("sfx/Lush_-_Si.ogg");
	P_LoadSound("sfx/Lush_-_Do_high.ogg");

	//Enemy sounds load
	E_LoadSound("sfx/Bot_-_Do_low.ogg");
	E_LoadSound("sfx/Bot_-_Re.ogg");
	E_LoadSound("sfx/Bot_-_Mi.ogg");
	E_LoadSound("sfx/Bot_-_Pa.ogg");
	E_LoadSound("sfx/Bot_-_Sol.ogg");
	E_LoadSound("sfx/Bot_-_Ra.ogg");
	E_LoadSound("sfx/Bot_-_Si.ogg");
	E_LoadSound("sfx/Bot_-_Do_high.ogg");

	//Gate sounds load
	G_LoadSound("sfx/Choir_-_Do_low.ogg");
	G_LoadSound("sfx/Choir_-_Re.ogg");
	G_LoadSound("sfx/Choir_-_Mi.ogg");
	G_LoadSound("sfx/Choir_-_Pa.ogg");
	G_LoadSound("sfx/Choir_-_Sol.ogg");
	G_LoadSound("sfx/Choir_-_Ra.ogg");
	G_LoadSound("sfx/Choir_-_Si.ogg");
	G_LoadSound("sfx/Choir_-_Do_high.ogg");
	G_LoadSound("sfx/end_bgm.ogg");

	//Shoot sounds load
	Shoot_LoadSound("sfx/shoot.ogg");

	//BGM sounds load
	BGM_LoadSound("sfx/main_bgm.ogg");
}