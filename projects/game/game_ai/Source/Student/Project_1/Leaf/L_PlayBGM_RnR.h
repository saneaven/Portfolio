#pragma once
#include "BehaviorNode.h"

class L_PlayBGM_RnR : public BaseNode<L_PlayBGM_RnR>
{
protected:
    virtual void on_enter() override;
    virtual void on_exit() override;
};