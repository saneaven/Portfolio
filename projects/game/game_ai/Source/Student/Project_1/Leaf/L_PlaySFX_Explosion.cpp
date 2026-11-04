#include "pch.h"
#include "L_PlaySFX_Explosion.h"

void L_PlaySFX_Explosion::on_enter()
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\explosion.wav");
	BehaviorNode::on_leaf_enter();
	on_success();
}
