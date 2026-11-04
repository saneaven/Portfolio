#pragma once
#include "BehaviorNode.h"

class L_Wander : public BaseNode<L_Wander>
{
public:
    L_Wander();

protected:
    float wanderRad;
    float wanderdist;
    float maxDelta;
    float currentAngle;
    float currentDelta;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};