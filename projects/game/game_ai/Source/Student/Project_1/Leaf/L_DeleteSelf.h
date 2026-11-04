#pragma once
#include "BehaviorNode.h"

class L_DeleteSelf : public BaseNode<L_DeleteSelf>
{
protected:
    virtual void on_enter() override;
};