#include "pch.h"
#include "L_CircularFormation.h"
#include "A_CirclularFormationInfo.h"


L_CircularFormation::L_CircularFormation(): circularAngle(0.f)
{
}

void L_CircularFormation::on_enter()
{
	const auto& blackboard = agent->get_blackboard();
	circularAngle = blackboard.get_value<float>("Circular Position");
	runSpeed = agent->get_movement_speed();
	circularFormatManager.SetAgent(agent); // Any way only one agent will assigned

	BehaviorNode::on_leaf_enter();
}

float speedAdd = PI/4.f;

void L_CircularFormation::on_update(float dt)
{
	circularFormatManager.Update(dt, agent);
	circularFormatManager.SetTurnSpeed(circularFormatManager.GetTurnSpeed() + (speedAdd * dt), agent);

	float newAngle = circularFormatManager.GetAngle();
	newAngle += circularAngle;

	float radius = circularFormatManager.GetRadius();

	Vec3 previousPos = agent->get_position();
	Vec2 targetPos2D = radius * Vec2(cos(newAngle), sin(newAngle));

	Vec3 centerPos = circularFormatManager.GetCenterPos();
	Vec3 targetPos = Vec3(targetPos2D.x, previousPos.y, targetPos2D.y) + centerPos;
	
	float deltaSq = Vec3::DistanceSquared(targetPos, previousPos);

	float realRunSpeed = runSpeed * dt;
	float runSpeedSq = realRunSpeed * realRunSpeed;

	if ( deltaSq > runSpeedSq * 4 ) // If destination is so far, Boost
		agent->set_movement_speed(runSpeed * 20);
	else
		agent->set_movement_speed(runSpeed);

	agent->move_toward_point(targetPos, dt);

	display_leaf_text();
}

void L_CircularFormation::on_exit()
{
	agent->set_movement_speed(runSpeed); // I DON'T UNDERSTAND WHY THIS IS NOT RUNNING < Now I find it.
	BehaviorNode::on_exit();
}