#include "pch.h"
#include "L_PlayBGM_RnR.h"

void L_PlayBGM_RnR::on_enter()
{
	audioManager->PlayMusic(L"Assets\\Audio\\RoundAndRound.wav");
	
	BehaviorNode::on_leaf_enter();
}

void L_PlayBGM_RnR::on_exit()
{
	audioManager->StopMusic(L"Assets\\Audio\\RoundAndRound.wav");
}
