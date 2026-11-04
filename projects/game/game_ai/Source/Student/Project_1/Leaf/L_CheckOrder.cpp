#include "pch.h"
#include "L_CheckOrder.h"

L_CheckOrder::L_CheckOrder(): leader(NULL)
{
}

void L_CheckOrder::on_enter()
{
	const auto& blackboard = agent->get_blackboard();
	size_t leaderID = blackboard.get_value<size_t>("Leader ID");
	
	leader = static_cast<BehaviorAgent*>(agents->get_all_agents_by_type("ParticipantLeader")[leaderID]);
	BehaviorNode::on_leaf_enter();
}

void L_CheckOrder::on_update(float dt)
{
	const auto& blackboard = leader->get_blackboard();
	std::string order = blackboard.get_value<std::string>("Order");
	
	if (order != "")
	{
		on_success();
	}
	else
	{
		on_failure();
	}
}

