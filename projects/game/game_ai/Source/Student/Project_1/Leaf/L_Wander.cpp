#include "pch.h"
#include "L_Wander.h"

L_Wander::L_Wander(): wanderRad(0.), wanderdist(0.), maxDelta(0.), currentAngle(0.), currentDelta(0.)
{
}

void L_Wander::on_enter()
{
	wanderRad = 10.f;
	maxDelta = PI/45.f;
	wanderdist = wanderRad * 100.f;
	Vec3 fwdVec = agent->get_forward_vector();
	currentAngle = atan2(fwdVec.z, fwdVec.x); //Set Angle forward
	currentDelta = 0.;

	BehaviorNode::on_leaf_enter();
}

Vec2 CENTER_POS = { 50., 50. };
float CENTER_FACTOR = .1;

void L_Wander::on_update(float dt)
{
	float deltaAccel = RNG::range<float>(-maxDelta, maxDelta);
	
	currentDelta += deltaAccel * dt;
	currentAngle += currentDelta;
	
	const Vec3& fwdVec = agent->get_forward_vector();
	Vec2 fwdVec2D = Vec2(fwdVec.x, fwdVec.z);
	fwdVec2D.Normalize();

	const Vec3& currentPos = agent->get_position();
	Vec2 currentPos2D = Vec2(currentPos.x, currentPos.z);
	
	Vec2 centerVec = CENTER_POS - currentPos2D; // This is a vector that NPCs can wander in screen.

	Vec2 targetPos2D = wanderRad * Vec2(cosf(currentAngle), sinf(currentAngle));
	targetPos2D += currentPos2D + wanderdist * fwdVec2D + CENTER_FACTOR * centerVec;
	Vec3 targetPos(targetPos2D.x, currentPos.y, targetPos2D.y);

	agent->move_toward_point(targetPos, dt);

	display_leaf_text();
}
