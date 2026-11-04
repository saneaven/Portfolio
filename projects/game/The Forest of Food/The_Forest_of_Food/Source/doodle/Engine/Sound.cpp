/*
File name : Sound.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "sound.h"
#include "Engine.h"

SFX::SFX(const std::string& filePath, bool soundRelative)
{
	sound.setBuffer(*(Engine::GetSoundManager().LoadSFX(filePath)));
	sound.setRelativeToListener(soundRelative);
}

void SFX::Play()
{
	sound.play();
}
void SFX::PlayOnly()
{
	sound.stop();
	sound.play();
}
bool SFX::isRelative()
{
	return sound.isRelativeToListener();
}

BGM::BGM(const std::string& filePath, bool loop)
{
	music = Engine::GetSoundManager().LoadBGM(filePath);
	music->setLoop(loop);
}

void BGM::Play()
{
	music->stop();
	music->play();
}
void BGM::Stop()
{
	music->stop();
}
bool BGM::isLoop()
{
	return music->getLoop();
}
