#include "pch.h"
#include "L_OrderFollow.h"


void L_OrderFollow::on_enter()
{
	auto& blackboard = agent->get_blackboard();
	blackboard.set_value<std::string>("Order", "follow");

	BehaviorNode::on_leaf_enter();
	on_success();
}
