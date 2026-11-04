#pragma once
#include "BehaviorNode.h"

class L_MoveToLeader: public BaseNode<L_MoveToLeader>
{
public:
    L_MoveToLeader();

protected:
    BehaviorAgent* leader;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};