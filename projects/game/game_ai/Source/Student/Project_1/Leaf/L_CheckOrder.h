#pragma once
#include "BehaviorNode.h"

class L_CheckOrder : public BaseNode<L_CheckOrder>
{
public:
    L_CheckOrder();

protected:
    BehaviorAgent* leader;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};