#pragma once
#include "BehaviorNode.h"

class L_MoveForward : public BaseNode<L_MoveForward>
{
public:
    L_MoveForward();

protected:
    BehaviorAgent* target;
    Vec3 fwdVec;
    float speed;
    float timer;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};