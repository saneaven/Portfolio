#pragma once
#include "BehaviorNode.h"

class L_CircularFormation : public BaseNode<L_CircularFormation>
{
public:
    L_CircularFormation();

protected:
    float circularAngle; // radian
    float runSpeed;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
    virtual void on_exit() override; 
};