#include "pch.h"
#include "L_Explode.h"

L_Explode::L_Explode() :currentSize(1), timer(0)
{
}

void L_Explode::on_enter()
{
	BehaviorNode::on_leaf_enter();
	currentSize = agent->get_scaling().x;
}

constexpr float explodeSpeed = 15.f;
constexpr float MAXTIME = .1f;

void L_Explode::on_update(float dt)
{
	currentSize += explodeSpeed * dt;
	timer += dt;
	
	agent->set_scaling(currentSize);

	if (timer > MAXTIME)
		on_success();

	display_leaf_text();
}
