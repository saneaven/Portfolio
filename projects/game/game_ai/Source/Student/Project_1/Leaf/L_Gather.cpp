#include "pch.h"
#include "L_Gather.h"

L_Gather::L_Gather(): partner(NULL), middlePoint(0, 0, 0), moveSpeed(0)
{
}

void L_Gather::on_enter()
{
	const auto& blackboard = agent->get_blackboard();
	size_t ID = blackboard.get_value<size_t>("Partner ID");
	partner = static_cast<BehaviorAgent *>(agents->get_all_agents_by_type("ParticipantFollower")[ID]);
	middlePoint = (agent->get_position() + partner->get_position())/2;
	moveSpeed = agent->get_movement_speed();
	agent->set_movement_speed(moveSpeed * 5);

	BehaviorNode::on_leaf_enter();
}

void L_Gather::on_update(float dt)
{
	bool result = agent->move_toward_point(middlePoint, dt);
	if (result)
	{
		agent->set_movement_speed(moveSpeed*10);
		on_success();
	}

	display_leaf_text();
}

