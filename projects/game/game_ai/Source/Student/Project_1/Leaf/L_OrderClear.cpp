#include "pch.h"
#include "L_OrderClear.h"

void L_OrderClear::on_enter()
{
	auto& blackboard = agent->get_blackboard();
	blackboard.set_value<std::string>("Order", "");

	BehaviorNode::on_leaf_enter();
	on_success();
}
