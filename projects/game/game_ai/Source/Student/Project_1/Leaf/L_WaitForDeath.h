#pragma once
#include "pch.h"
#include "BehaviorNode.h"

class L_WaitForDeath : public BaseNode<L_WaitForDeath>
{
public:
    L_WaitForDeath();

protected:
    Vec3 originalPos;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};