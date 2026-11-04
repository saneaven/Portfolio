#include "pch.h"
#include "L_WaitForDeath.h"

L_WaitForDeath::L_WaitForDeath(): originalPos()
{
}

void L_WaitForDeath::on_enter()
{
	originalPos = agent->get_position();

	BehaviorNode::on_leaf_enter();
}

float range = 1.f;

void L_WaitForDeath::on_update(float dt)
{
	Vec3 shakeVector(RNG::range(-range, range), RNG::range(-range, range), RNG::range(-range, range));

	agent->move_toward_point(originalPos + shakeVector, dt);

	display_leaf_text();
}