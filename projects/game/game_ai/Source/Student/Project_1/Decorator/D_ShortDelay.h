#pragma once
#include "BehaviorNode.h"

class D_ShortDelay : public BaseNode<D_ShortDelay>
{
public:
    D_ShortDelay();

protected:
    float delay;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};