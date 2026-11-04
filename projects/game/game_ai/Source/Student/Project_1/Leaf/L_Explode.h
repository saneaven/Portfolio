#pragma once
#include "BehaviorNode.h"

class L_Explode : public BaseNode<L_Explode>
{
public:
    L_Explode();

protected:
    float currentSize;
    float timer;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};