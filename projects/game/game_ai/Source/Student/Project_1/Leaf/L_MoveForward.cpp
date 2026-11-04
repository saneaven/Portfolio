#include "pch.h"
#include "L_MoveForward.h"

L_MoveForward::L_MoveForward():target(NULL),timer(0), fwdVec(), speed(0)
{
}

void L_MoveForward::on_enter()
{
	target = static_cast<BehaviorAgent*>(agents->get_all_agents_by_type("ParticipantLeader")[0]);
	fwdVec = agent->get_forward_vector();
	speed = agent->get_movement_speed();
	BehaviorNode::on_leaf_enter();
}

const float MAXTIME = 5.f;
const float epsilon = 9.f;

void L_MoveForward::on_update(float dt)
{
	agent->set_position(agent->get_position() + dt * speed * fwdVec);
	Vec3 currentPos = agent->get_position();
	Vec3 targetPos = target->get_position();
	Vec3 estimateHitPos{ targetPos.x, currentPos.y, targetPos.z };

	float distanceSq = Vec3::DistanceSquared(currentPos, estimateHitPos);

	if (distanceSq < epsilon)
	{
		on_success();
	}
	if (timer > MAXTIME)
	{
		on_failure();
	}

	timer += dt;

	display_leaf_text();
}
