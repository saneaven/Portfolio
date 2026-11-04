#include "pch.h"
#include "L_PlaySFX_Gun.h"

void L_PlaySFX_Gun::on_enter()
{
	audioManager->PlaySoundEffect(L"Assets\\Audio\\gun.wav");
	BehaviorNode::on_leaf_enter();
	on_success();
}
