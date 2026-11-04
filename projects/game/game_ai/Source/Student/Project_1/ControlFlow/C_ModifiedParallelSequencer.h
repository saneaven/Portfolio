#pragma once
#include "BehaviorNode.h"

class C_ModifiedParallelSequencer : public BaseNode<C_ModifiedParallelSequencer>
{

    void ForceOtherFailed(BehaviorNode*);
protected:
    virtual void on_update(float dt) override;
};