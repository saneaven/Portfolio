#pragma once
#include "BehaviorNode.h"

class L_SpawnAgent : public BaseNode<L_SpawnAgent>
{
protected:
    virtual void on_enter() override;
};