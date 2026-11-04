#pragma once
#include "BehaviorNode.h"

class L_OrderFollow : public BaseNode<L_OrderFollow>
{
protected:
    virtual void on_enter() override;
};