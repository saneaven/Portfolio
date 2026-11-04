#pragma once
#include "BehaviorNode.h"

class L_Gather : public BaseNode<L_Gather>
{
public:
    L_Gather();

protected:
    BehaviorAgent* partner;
    Vec3 middlePoint;
    float moveSpeed;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};