#include "pch.h"
#include "L_PlayBGM_PinkSolder.h"

void L_PlayBGM_PinkSolder::on_enter()
{
	audioManager->PlayMusic(L"Assets\\Audio\\PinkSolders.wav");

	BehaviorNode::on_leaf_enter();
	on_success();
}