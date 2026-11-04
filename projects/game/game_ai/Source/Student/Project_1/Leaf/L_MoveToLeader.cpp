#include "pch.h"
#include "L_MoveToLeader.h"

L_MoveToLeader::L_MoveToLeader(): leader(NULL)
{
}

void L_MoveToLeader::on_enter()
{
	leader = static_cast<BehaviorAgent*>(agents->get_all_agents_by_type("ParticipantLeader")[0]);
	BehaviorNode::on_leaf_enter();
}

float minDistanceSq = 400.f;

void L_MoveToLeader::on_update(float dt)
{
	Vec3 leaderPos = leader->get_position();

	float distanceSq = Vec3::DistanceSquared(leaderPos, agent->get_position());
	if (distanceSq < minDistanceSq)
	{
		on_success();
	}
	else
	{
		agent->move_toward_point(leaderPos, dt);
	}

	display_leaf_text();
}
