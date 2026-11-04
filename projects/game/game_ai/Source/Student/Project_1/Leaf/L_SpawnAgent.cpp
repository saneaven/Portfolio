#include "pch.h"
#include "L_SpawnAgent.h"
#include "Projects/ProjectOne.h"

const float HOWFAR = 50.f;

void L_SpawnAgent::on_enter()
{
    auto triangle = agents->create_behavior_agent("AgentTriangle", BehaviorTreeTypes::AgentTriangle);
    Vec3 randomPos = RNG::world_position();
    Vec3 directionVec = randomPos - agent->get_position();
    directionVec.Normalize();

    triangle->set_position(RNG::world_position() + HOWFAR * directionVec);
    triangle->set_yaw(RNG::range(-PI, PI));
    triangle->set_color({ 244./255., 71./255., 134./255. });

    BehaviorNode::on_leaf_enter();
    on_success();
}
