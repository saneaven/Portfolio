#include "pch.h"
#include "L_Shoot.h"
#include "Projects/ProjectOne.h"

L_Shoot::L_Shoot()
{
	
}

void L_Shoot::on_enter()
{

	BehaviorNode::on_leaf_enter();

}

void L_Shoot::on_update(float dt)
{
	auto bullet = agents->create_behavior_agent("Bullet", BehaviorTreeTypes::Bullet, Agent::AgentModel::Car);
	bullet->set_position(agent->get_position() + Vec3(0,20,0));
	bullet->set_color({ 48. / 255., 3. / 255., 9. / 255. });
	bullet->set_movement_speed(100);
	bullet->set_scaling(0.1);
	bullet->set_yaw(agent->get_yaw());

	on_success();
	display_leaf_text();
}
