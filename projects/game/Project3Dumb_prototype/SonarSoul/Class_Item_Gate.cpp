/*
File name : Class_Item_Gate.cpp
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Haneul Lee
¡°All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.¡±
*/

#include "classes.h"
#include "basicFunc.h"
#include "SFX.h"

int ITEMSIZE = 10;



Item::Item(Vec2 pos, int t) : Object(pos, (float)ITEMSIZE, (float)ITEMSIZE, 1)
{
	switch (t)
	{
	case 0:
		syllable = Syllable::Do;
		break;
	case 1:
		syllable = Syllable::Re;
		break;
	case 2:
		syllable = Syllable::Mi;
		break;
	case 3:
		syllable = Syllable::Fa;
		break;
	case 4:
		syllable = Syllable::Sol;
		break;
	case 5:
		syllable = Syllable::La;
		break;
	case 6:
		syllable = Syllable::Ti;
		break;
	case 7:
		syllable = Syllable::Do_high;
		break;
	default:
		syllable = Syllable::Do;
		break;
	}
}

void Item::interact(Player& player)
{
	switch (syllable)
	{
	case Syllable::Do:
		player.a_note = true;
		break;
	case Syllable::Re:
		player.s_note = true;
		break;
	case Syllable::Mi:
		player.d_note = true;
		break;
	case Syllable::Fa:
		player.f_note = true;
		break;
	case Syllable::Sol:
		player.q_note = true;
		break;
	case Syllable::La:
		player.w_note = true;
		break;
	case Syllable::Ti:
		player.e_note = true;
		break;
	case Syllable::Do_high:
		player.r_note = true;
		break;
	}
}

void Item::draw(Camera camera)
{
	using namespace doodle;

	const float& magnification = (float)camera.getMagnification();
	const float thisX = (float)(this->getPosition().xValue() - camera.getPosition().xValue()) * magnification;
	const float thisY = (float)(this->getPosition().yValue() - camera.getPosition().yValue()) * magnification;

	push_settings();
	set_fill_color(HexColor{ this->getColor() });
	draw_rectangle(thisX, thisY, (float)this->getWidth() * magnification, (float)this->getHeight() * magnification);
	set_font_size((float)this->getHeight() * 2);
	string syl;
	switch (this->syllable)
	{
	case Syllable::Do:
		syl = "A";
		break;
	case Syllable::Re:
		syl = "S";
		break;
	case Syllable::Mi:
		syl = "D";
		break;
	case Syllable::Fa:
		syl = "F";
		break;
	case Syllable::Sol:
		syl = "Q";
		break;
	case Syllable::La:
		syl = "W";
		break;
	case Syllable::Ti:
		syl = "E";
		break;
	case Syllable::Do_high:
		syl = "R";
		break;
	}
	draw_text(syl, thisX, thisY);
	pop_settings();
}


Gate::Gate() : Object(Vec2(0, 0), BLOCKSIZE, BLOCKSIZE, 1) {}

Gate::Gate(Vec2 pos, std::vector<Syllable> k) : Object(pos, BLOCKSIZE, BLOCKSIZE, 1), key(k)
{
	setColor(0xFFFF00FF);
}

void Gate::play_gate_song()
{
	for (auto& sound : G_sounds)
	{
		if (sound.getStatus() != sf::SoundSource::Playing)
		{
			sound.setBuffer(G_soundBuffers[8]);
			sound.play();
		}
	}
	G_sounds.emplace_back(G_soundBuffers[8]);
	G_sounds.back().play();

	isEnding = false;
}

void Gate::play_song()
{
	for (int i = 0; i < this->key.size(); ++i)
	{
		setTimer([this , i]()
			{
				play_note(this->key[i]);
			}, i * 300);
	}

}

void Gate::play_note(Syllable note)
{
	for (auto& sound : G_sounds)
	{
		if (sound.getStatus() != sf::SoundSource::Playing)
		{
			sound.setBuffer(G_soundBuffers[(int)note]);
			sound.play();
			std::cout << "sound test\n";
		}
	}


	G_sounds.emplace_back(G_soundBuffers[(int)note]);
	G_sounds.back().play();
	return;
}

void Gate::setKey(const std::vector<Syllable>& keyVector)
{
	this->stack.clear();
	this->key = keyVector;
}
bool Gate::addStack(const Syllable& stackSyllable)
{
	this->stack.push_back(stackSyllable);
	int size = (int)stack.size();
	int startIdx = size - (int)key.size();

	if (startIdx < 0)
		return false;

	for (Syllable i : this->stack)
		cout << (int)i;
	cout << endl;
	for (Syllable i : this->key)
		cout << (int)i;
	cout << endl;

	for (int i = startIdx; i < size; ++i)
	{
		if (stack[i] != key[i - startIdx])
		{
			cout << (int)stack[i] << " != " << (int)key[i-startIdx];
			return false;
		}
	}                        

	Game.CurrentScreen = Screen::Ending;
	return true;
}

GateSonar::GateSonar(Vec2 p, double r, Syllable s) : Sonar{ p,r,s }
{
	setColor(0x22FF44FF);
};


bool GateSonar::update(Gate& gate)
{
	uint32_t color_c = this->getColor() & 0xFFFFFF00;
	this->alpha;
	this->circle_radius += SONARSPEED * 1.25 * doodle::DeltaTime;
	alpha -= SONARSPEED * doodle::DeltaTime;
	if (alpha < 0)
		return false;

	if (Circlecollision(gate))
		return false;

	this->setColor(color_c | (int)alpha);
	return true;
}

void GateSonar::draw(Camera camera)
{
	using namespace doodle;
	push_settings();
	set_outline_color(HexColor{ this->getColor() });
	set_outline_width(5.f);
	no_fill();
	set_outline_width(5);
	draw_ellipse((float)(this->getPosition().x - camera.getPosition().x) * (float)camera.getMagnification(), (float)(this->getPosition().y - camera.getPosition().y) * (float)camera.getMagnification(), (float)circle_radius * 2 * (float)camera.getMagnification());
	pop_settings();
}

bool GateSonar::Circlecollision(Gate& gate)
{
	Direction collisionDirection = Direction::notD;
	if (this->collisionCirRect(gate, collisionDirection))
	{
		if (gate.addStack(this->syllable))
		{
			Game.CurrentScreen = Screen::Ending;
		}
		cout << "test;" << (int)this->syllable;
		return true;
	}
	return false;
}