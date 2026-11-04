/*
	File name: classes.h
	Project name: Painter
	Course: GAM100
	Term: Fall 2019
	primary author: Minjae Kyung
	secondary author: Haeun Park
	All content © 2019 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <vector>
#include <iostream>

constexpr double PI = 3.1415926535897;
constexpr int BLOCKSIZE = 50;
constexpr int BULLETSIZE = 5;
constexpr double G = 9.80665 * 50;
constexpr double STEPFORCE = 300;
constexpr double MAXSPEED = 300;
constexpr double JUMPFORCE = 350;
constexpr double BULLETPOWER = 300;
constexpr double EPSILON = 0.001;
enum class Direction { notD = -1, center, left, right, up, down, edge, upLeft, upRight, downRight, downLeft };


class Vec2
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
	const Vec2 operator+(const Vec2& other) const;
	const Vec2 operator-(const Vec2& other) const;
	const Vec2 operator*(const double& scalar) const;
	const Vec2 operator/(const double& scalar) const;

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
};

class Camera;
class Item;

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
	void setForce(Vec2 f);
	void resetForce();

	void gravity();
	void friction();

	Direction collisionCheck(Object& checkObject);
	void collisionInteract(Direction direction);
	void WallCollisionCheck();

public:
	Object(Vec2 pos) : position(pos) {}
	Object(double x, double y) : position(Vec2(x, y)) {}
	Object(Vec2 pos, float w, float h, int m) : position(pos), width(w), height(h), mass(m) {}
	Object(double x, double y, float w, float h, int m) : position(Vec2(x, y)), width(w), height(h), mass(m) {}

	void setColor(uint32_t s_color);
	void addColor(uint32_t a_color);
	void hitbyball(uint32_t col);

	const Vec2 getPosition() const;
	const Vec2 getVelocity() const;
	const Vec2 getLawVelocity() const;
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

	void draw();
	void draw(Camera camera);
};

class Block : public Object
{

public:
	Block(double x, double y) : Object(x, y, BLOCKSIZE, BLOCKSIZE, 1) {}
	Block(Vec2 pos) : Object(pos, BLOCKSIZE, BLOCKSIZE, 1) {}
	Block(Vec2 pos, char c);

};


class Enemy : public Object
{
	int type = 0;
	Vec2 walkForce;
	Direction walkDirection = Direction::left;

	void initEnemyByType(int t);

public:
	Enemy(Vec2 pos, int t);
	Enemy(double x, double y, int t);
	Enemy(Vec2 pos, float wid, float h, int w, int t) : Object(pos, wid, h, w), type(t) {}
	Enemy(double x, double y, float wid, float h, int w, int t) : Object(x, y, wid, h, w), type(t) { setColor(0xff0000ff); }



	void walk();
	void update(const std::vector<Block>& blocks);
	void detectBlock(const std::vector<Block>& blocks);
};

class Bullet : public Object
{
public:
	Bullet(Vec2 p, Vec2 v, Direction d, uint32_t c);
	Bullet(double x, double y, uint32_t c);
	Bullet(double x, double y, Vec2 f, uint32_t c);

	bool update(std::vector<Enemy>& enemies, std::vector<Block>& blocks);
};

class Player : public Object
{
	std::string player_name = "";
	uint32_t ball_color = 0xFF0000FF;

	int health = 1;
	int ballStorage = 20;

	bool isRunning = false;
	bool isthrowing = false;

	void keyCheck(std::vector<Bullet>& Bullets);

public:
	Player(Vec2 pos) : Object(pos) { setColor(0x000000FF); }
	Player(double x, double y) : Object(x, y) { setColor(0x000000FF); }
	Player(Vec2 pos, float wid, float h, int w) : Object(pos, wid, h, w) { setColor(0x000000FF); }
	Player(double x, double y, float wid, float h, int w) : Object(x, y, wid, h, w) { setColor(0x000000FF); }

	bool isLeft = false, isRight = false;
	Direction staringDirection = Direction::right;
	bool isJump = false;
	bool isShoot = false;
	bool isOnGround = false;
	bool isDead = false;
	bool isStageClear = false;

	void update(std::vector<Bullet>& Bullets, std::vector<Enemy>& enemies, std::vector<Block>& blocks, std::vector<Item>& items);
	void setBallColor(uint32_t col);
	void drawBall(Camera camera);
	int getBallStorage() const;
	uint32_t getBallColor() const;
};

class Item :public Object
{
	int type = 0;

public:
	Item(Vec2 p, int type);

	void interact(Player& player);
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

class Button {

public:
	Button(std::string tex, float x, float y, float width, float height);
	Button(std::string tex, float x, float y, float width, float height, float fnt_size);
	bool isMouseOnButton();
	void draw();
	void setFontSize(float size);

private:
	std::string text = "";
	float pos_x = 0;
	float pos_y = 0;
	float button_width = 30;
	float button_height = 30;
	float fnt_size = button_width / text.size();

};