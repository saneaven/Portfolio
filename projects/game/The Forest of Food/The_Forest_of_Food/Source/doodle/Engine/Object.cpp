/*
File name : Object.cpp
Game name : The forest of food
the course name  : GAM150
the term : Spring 2020
Author(primary) : Minjae Kyung
All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/

#include "Object.h"
#include "Line.h"

Direction Object::CollisionCheck(const Object* other, double dt ) const //Collision direction between object(notD is not Collisioned)
{
	if (hitbox.isOverlapped(other->hitbox))
	{
		Vector2D relativeVelocity = velocity - other->velocity;
		Vector2D collisionEdge;

		relativeVelocity *= dt;

		Direction edgeDirection = Direction::notD;

		if(other->hitbox.isPointInside(hitbox.GetPositionOf(Direction::leftUp)))
		{
			collisionEdge = hitbox.GetPositionOf(Direction::leftUp);
			edgeDirection = Direction::leftUp;
		}
		else if (other->hitbox.isPointInside(hitbox.GetPositionOf(Direction::rightUp)))
		{
			collisionEdge = hitbox.GetPositionOf(Direction::rightUp);
			edgeDirection = Direction::rightUp;
		}
		else if (other->hitbox.isPointInside(hitbox.GetPositionOf(Direction::rightDown)))
		{
			collisionEdge = hitbox.GetPositionOf(Direction::rightDown);
			edgeDirection = Direction::rightDown;
		}
		else if (other->hitbox.isPointInside(hitbox.GetPositionOf(Direction::leftDown)))
		{
			collisionEdge = hitbox.GetPositionOf(Direction::leftDown);
			edgeDirection = Direction::leftDown;
		}

		using namespace std;
		cout << "Collision Edge : " << collisionEdge << endl;
		cout << "this hitbox left down : " << hitbox.GetPositionOf(Direction::leftDown) << endl;
		cout << "this hitbox right up : " << hitbox.GetPositionOf(Direction::rightUp) << endl;
		cout << "other hitbox left down : " << other->hitbox.GetPositionOf(Direction::leftDown) << endl;
		cout << "other hitbox right up : " << other->hitbox.GetPositionOf(Direction::rightUp) << endl;


		if (isCrossing({ collisionEdge, collisionEdge - relativeVelocity }, 
			{ other->hitbox.GetPositionOf(Direction::leftUp), other->hitbox.GetPositionOf(Direction::rightUp) }) 
			&& 
			(edgeDirection == Direction::rightDown || edgeDirection == Direction::leftDown))
		{
			return Direction::up;
		}
		else if (isCrossing({ collisionEdge, collisionEdge - relativeVelocity }, 
			{ other->hitbox.GetPositionOf(Direction::rightUp), other->hitbox.GetPositionOf(Direction::rightDown) })
			&&
			(edgeDirection == Direction::leftUp || edgeDirection == Direction::leftDown))
		{
			return Direction::right;
		}
		else if (isCrossing({ collisionEdge, collisionEdge - relativeVelocity },
			{ other->hitbox.GetPositionOf(Direction::rightDown), other->hitbox.GetPositionOf(Direction::leftDown) })
			&&
			(edgeDirection == Direction::rightUp || edgeDirection == Direction::leftUp))
		{
			return Direction::down;
		}
		else if (isCrossing({ collisionEdge, collisionEdge - relativeVelocity },
			{ other->hitbox.GetPositionOf(Direction::leftDown), other->hitbox.GetPositionOf(Direction::leftUp) })
			&&
			(edgeDirection == Direction::rightUp || edgeDirection == Direction::rightDown))
		{
			return Direction::left;
		}

		return Direction::center;
	}

	return Direction::notD;
}

void Object::CollisionInteractWith(Object*, Direction){}

void Object::SetName(std::string objectName){
	name = objectName;
}


std::string Object::GetName(){
	return name;
}


void Object::HitboxDraw(Camera CameraMatrix)
{
	hitbox.Draw(CameraMatrix * TranslateMatrix(hitbox.GetPositionOf(Direction::center)));
}

const Rectangle& Object::GetHitbox() const
{
	return hitbox;
}

void Object::HitboxUpdate()
{
	hitbox.SetPosition(position + sprite.GetHotSpot(0));
}

Vector2D<int> Object::GetHotSpot(int index)
{
	return sprite.GetHotSpot(index);
}

void Object::Draw(Camera CameraMatrix)
{
	//HitboxDraw(CameraMatrix);
	sprite.Draw(CameraMatrix * objectMatrix);
}

void Object::AddSFX(std::string nameSFX, std::string filePath)
{
	soundEffects.insert({ nameSFX, new SFX{filePath} });
}
void Object::PlaySFX(std::string nameSFX)
{
	soundEffects[nameSFX]->Play();
}
void Object::RePlaySFX(std::string nameSFX)
{
	soundEffects[nameSFX]->PlayOnly();
}
void Object::Unload()
{
	for (std::pair<std::string, SFX*> sfx : soundEffects)
	{
		delete sfx.second;
	}
	soundEffects.clear();
}

