#include "pch.h"
#include "L_DeleteSelf.h"

void L_DeleteSelf::on_enter()
{
	agents->destroy_agent(agent);
	on_success();
	BehaviorNode::on_leaf_enter();
}
