#pragma once
#include "BehaviorNode.h"

class L_Shoot : public BaseNode<L_Shoot>
{
    float delay;

public:
    L_Shoot();
    
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};