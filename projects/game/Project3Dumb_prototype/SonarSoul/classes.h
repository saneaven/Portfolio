/*
File name : classes.h
Game name : Sonar Soul
the course name  : GAM100
the term : Fall 2019
Author : MinJae Kyung, Haneul Lee, Hoseob Jeong
“All content (C) 2019 DigiPen (USA) Corporation, all rights reserved.”
*/

#pragma once
#include <vector>
#include <iostream>
#include <doodle/doodle.hpp>


constexpr double PI = 3.1415926535897;
constexpr int BLOCKSIZE = 50;
constexpr int BULLETSIZE = 5;
constexpr double G = 9.80665 * 100;
constexpr double STEPFORCE = 1000;
constexpr double MAXSPEED = 300;
constexpr double JUMPFORCE = 350;
constexpr double BULLETPOWER = 400;
constexpr double SHOOTDELAY = 300;
constexpr double RELOADTIME = 1000;
constexpr double EPSILON = 0.001;
constexpr double SONARSPEED = 500;
enum class Direction { notD = -1, center, left, right, up, down, edge, upLeft, upRight, downRight, downLeft };

enum Screen {Main_menu = 1 , PlayGame, Credit, Reload, Ending};
enum class Syllable { Do = 0, Re, Mi, Fa, Sol, La, Ti, Do_high };
extern inline int isEnding = false;

struct Vec2
{
	double x = 0;
	double y = 0;

public:
	Vec2(double ix, double iy) : x(ix), y(iy) {}
	Vec2() : x(0), y(0) {}

	Vec2& operator+=(const Vec2& other);
	Vec2& operator-=(const Vec2& other);
	Vec2& operator*=(const double& scalar);
	Vec2& operator/=(const double& scalar);

	const double operator*(const Vec2& other) const;
	const double dotProduct(const Vec2& other) const;
	const double crossProduct(const Vec2& other) const;
	const double distance(const Vec2& other) const;
	const Vec2 operator+(const Vec2& other) const;
	const Vec2 operator-(const Vec2& other) const;
	const Vec2 operator-() const;
	const Vec2 operator*(const double& scalar) const;
	const Vec2 operator/(const double& scalar) const;

	bool isSimilarWith(const Vec2& other) const;
	bool operator==(const Vec2& other) const;
	bool operator!=(const Vec2& other) const;

	bool operator<(const Vec2& other) const;
	bool operator<=(const Vec2& other) const;
	bool operator>(const Vec2& other) const;
	bool operator>=(const Vec2& other) const;

	double getAngle() const;
	void setAngle(double radian);
	double getSlope() const;

	double getLength() const;
	void setLength(double length);
	void addLength(double length);

	double xValue() const;
	double yValue() const;

	void print();
	void draw(const Vec2& from);
};

class Camera;
class Item;
class Bullet;
class Sonar;
class Gate;
class GateSonar;

class Object
{
	Vec2 position = { 0, 0 };
	Vec2 velocity = { 0, 0 };
	Vec2 accelaration = { 0, 0 };
	Vec2 force = { 0, 0 };

	double width = 0;
	double height = 0;
	int mass = 1;

	uint32_t color = 0xFFFFFFFF;

protected:

	bool isHittedRight = false, isHittedLeft = false, isHittedUp = false, isHittedDown = false;

	double maxSpeedX = MAXSPEED;
	double maxSpeedY = MAXSPEED;
	bool isCoillsion = false;
	bool isGravity = true;

	void setMass(int m);
	void setWidth(double w);
	void setHeight(double h);
	void setPosition(Vec2 p);

	void move();
	void moveTo(Vec2 pos);
	void moveTo(double x, double y);
	void forcedMove(Vec2 pos);

	void accelerate();
	void setVelocity(Vec2 vel);
	void setSpeed(double speed);

	void setForceAccelation();
	void gainAccelation(Vec2 accel);
	void setAccelaration(Vec2 accel);


	void gainForce(Vec2 f);
	void resetForce();

	void resetPhysics();
	void stop();

	void gravity();
	void friction();

	bool collisionCheck(const Object& check, Direction& conllisionDirection, Vec2& focedMoveVector);
	void collisionInteract(Direction direction);

public:
	Object(Vec2 pos) : position(pos) {}
	Object(double x, double y) : position(Vec2(x, y)) {}
	Object(Vec2 pos, float w, float h, int m) : position(pos), width(w), height(h), mass(m) {}
	Object(double x, double y, float w, float h, int m) : position(Vec2(x, y)), width(w), height(h), mass(m) {}

	void setForce(Vec2 f);

	void setColor(uint32_t s_color);

	const Vec2 getPosition() const;
	const Vec2 getVelocity() const; // velocity per frame
	const Vec2 getLawVelocity() const; // velocity per second
	const double getSpeed() const;
	const Vec2 getAccelaration() const;
	const Vec2 getForce() const;
	const double getWidth() const;
	const double getHeight() const;
	const double getMass() const;
	const uint32_t getColor() const;

	const Vec2 getPositionOf(Direction direction) const;

	bool isPointInObject(Vec2 point) const;
	bool isPointInObject(double x, double y) const;
	void moveAnimation(Vec2 start, Vec2 end, int milisec);

	void draw();
	virtual void draw(Camera camera);
};

class Block : public Object
{

public:
	Block(double x, double y) : Object(x, y, BLOCKSIZE, BLOCKSIZE, 1) { setColor(0xDDCC88FF); }
	Block(Vec2 pos) : Object(pos, BLOCKSIZE, BLOCKSIZE, 1) { setColor(0xDDCC88FF); }
};


class Enemy : public Object
{
	int type = 0;
	Vec2 walkForce;
	Direction walkDirection = Direction::right;
	void initEnemyByType(int t);
	Syllable monster_note = Syllable::Do;

public:

	Enemy(Vec2 pos, int t);
	Enemy(double x, double y, int t);
	Enemy(Vec2 pos, float wid, float h, int w, int t) : Object(pos, wid, h, w), type(t) {}
	Enemy(double x, double y, float wid, float h, int w, int t) : Object(x, y, wid, h, w), type(t) { setColor(0xff0000ff); }
	Enemy(double x, double y, float wid, float h, int w, Syllable note) : Object(x, y, wid, h, w), type(0), monster_note(note) { setColor(0xff0000ff); }

	Syllable getNote();
	void play_note();
	void walk();
	void update(const std::vector<Block>& blocks);
};

class Player : public Object
{
	std::string player_name = "";
	uint32_t ball_color = 0xFF0000FF;

	double mouseAngle = 0;
	double mouseLength = 0;

	double shootDelay = 0;
	double reloadTime = 0;
	int health = 1;
	int magazine = 2;

	bool isRunning = false;
	bool isthrowing = false;
	
	void keyCheck(std::vector<Bullet>& Bullets);

public:
	Player(Vec2 pos) : Object(pos) { setColor(0xCCFF88FF); }
	Player(double x, double y) : Object(x, y) { setColor(0xCCFF88FF); }
	Player(Vec2 pos, float w, float h, int m) : Object(pos, w, h, m) { setColor(0xCCFF88FF); }
	Player(double x, double y, float w, float h, int m) : Object(x, y, w, h, m) { setColor(0xCCFF88FF); }

	bool isLeft = false, isRight = false;
	Direction staringDirection = Direction::right;
	Vec2 spawnPoint;

	std::vector<int> gateNotes{ 0,0,0,0,0,0,0,0 };
	bool isJump = false;
	bool isShoot = false;
	bool isOnGround = false;
	bool isDead = false;
	bool isStageClear = false;

	bool q_note = true;
	bool w_note = true;
	bool e_note = true;
	bool r_note = true;
	bool a_note = true;
	bool s_note = true;
	bool d_note = true;
	bool f_note = true;

	void crouch(bool isCrouch);
	void use_note(bool& note_key, Syllable buffer_index);
	void use_gateSonar(std::vector<GateSonar>& gateSonars, Syllable syl);
	void draw(Camera camera);
	void relativeMouseUpdate(const Camera& camera);
	void reloadBullet();
	void shootBullet(std::vector<Bullet>& Bullets);
	void update(std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items);
	void collision(std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items);
};

class Bullet : public Object
{
public:
	Bullet(Vec2 p, Vec2 v, Direction d, uint32_t c);
	Bullet(double x, double y, uint32_t c);
	Bullet(double x, double y, Vec2 f, uint32_t c);
	Bullet(Vec2 p, Vec2 v, Vec2 f);

	bool update(std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items, Gate gate);
	bool collision(std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items, Gate gate);
};

class Item : public Object
{
	Syllable syllable;

public:
	Item(Vec2 pos, int t);
	void interact(Player& player);
	void draw(Camera camera);
};

class Gate : public Object
{
	std::vector<Syllable> key;
	std::vector<Syllable> stack = { Syllable::Do };

	void play_note(Syllable);

public:
	Gate();
	Gate(Vec2 pos, std::vector<Syllable> k);

	void play_song();
	void play_gate_song();
	void setKey(const std::vector<Syllable>& keyVector);
	bool addStack(const Syllable& stackVector);
};



class Camera : public Object
{
	Object* const follow = NULL;
	double magnification = 1;

public:
	Camera(Vec2 pos);
	Camera(Object* const fol);
	Camera(Object* const fol, double mag);

	double getMagnification();
	void changeMagnification(double mag);
	void update();
};


class Sonar : public Object {

protected:
	double circle_radius = 50;
	Syllable syllable;
	double alpha = 0xFF;


public:

	Sonar(Vec2 p, Vec2 v, Vec2 a, Vec2 f);
	Sonar(Vec2 p, double r, Syllable s);
	
	void draw(Camera camera);
	bool update(std::vector<Enemy>& enemies);

	double get_circle_radius();
	
	void put_the_position(double,double);
	bool collisionCirRect(const Object& check, Direction& conllisionDirection);
	bool Circlecollision(std::vector<Enemy>& enemies);
};

class GateSonar : public Sonar
{
public:
	GateSonar(Vec2 p, double r, Syllable s);

	void draw(Camera camera);
	bool Circlecollision(Gate& gate);
	bool update(Gate& gate);
};

using namespace std;
struct HUD
{
	Vec2 position = { 0, 0 };
	double width = 0;
	double height = 0;
	string name;
	uint32_t color = 0x00000FF;
	bool isAnimation = false;

	HUD(string n, Vec2 pos, double w);
	HUD(string n, Vec2 pos, double w, double h);


	void setPosition(const Vec2& pos);
	const Vec2 getPosition() const;
	void moveAnimation(Vec2 start, Vec2 end, int milisec);
	void draw();
};



struct GAME {
	const float PLAY_BUTTON_X = -50.0f;
	const float PLAY_BUTTON_Y = -50.0f;
	const float	CREDIT_BUTTON_X = -50.0f;
	const float CREDIT_BUTTON_Y = -150.0f;
	const float BUTTON_WIDTH = 100.0f;
	const float BUTTON_HEIGHT = 40.0f;
	const float CREDIT_RANGE = 30.0f;
	const float CREDIT_START_Y = 200.0f;
	const float BACK_BUTTON_X = -50.0f;
	const float BACK_BUTTON_Y = -200.0f;

	bool isClicked = false;
	int CurrentScreen = 1;
	void main_menu();
	void credit();
	bool mouse_interact(const float&button_y,const float&width,const float&height);
	void DigiPen_screen();
	void isMouseClicked();
};

