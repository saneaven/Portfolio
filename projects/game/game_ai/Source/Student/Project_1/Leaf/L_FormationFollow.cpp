#include "pch.h"
#include "L_FormationFollow.h"
#include <string>

L_FormationFollow::L_FormationFollow(): deltaPos(0.), leader(NULL), runSpeed(-1.), epsilon(-1.)
{
	
}

void L_FormationFollow::on_enter()
{
	const auto& blackboard = agent->get_blackboard();
	deltaPos = blackboard.get_value<Vec3>("Delta Position");
	
	size_t leaderID = blackboard.get_value<size_t>("Leader ID");
	leader = static_cast<BehaviorAgent*>(agents->get_all_agents_by_type("ParticipantLeader")[leaderID]);

	runSpeed = agent->get_movement_speed();

	epsilon = 100.f;

	BehaviorNode::on_leaf_enter();
}

void L_FormationFollow::on_update(float dt)
{
	const auto& blackboard = leader->get_blackboard();
	std::string order = blackboard.get_value<std::string>("Order");

	const Vec3& leadertPos = leader->get_position();
	const Vec3& leaderFwdVec = leader->get_forward_vector();
	const Vec3& leaderUpVec = leader->get_up_vector();
	const Vec3& leaderRVec = leader->get_right_vector();

	Vec3 tempDelta = deltaPos.x * leaderFwdVec + deltaPos.y * leaderUpVec + deltaPos.z * leaderRVec; // Get relative Position
	Vec3 targetPos = leadertPos + tempDelta;
	

	float deltaSq = Vec3::DistanceSquared(agent->get_position(), tempDelta);
	
	float realRunSpeed = runSpeed * dt;
	float runSpeedSq = realRunSpeed * realRunSpeed;

	if ( deltaSq > runSpeedSq * 4) // If destination is so far, Boost
		agent->set_movement_speed(runSpeed * 10);
	else
		agent->set_movement_speed(runSpeed);

	agent->move_toward_point(targetPos, dt);

	if (order != "follow")
	{
		agent->set_movement_speed(runSpeed);
		on_success();
	}

	display_leaf_text();
}