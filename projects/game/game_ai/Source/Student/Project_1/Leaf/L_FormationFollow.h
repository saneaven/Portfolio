#pragma once
#include "BehaviorNode.h"

class L_FormationFollow : public BaseNode<L_FormationFollow>
{
public:
    L_FormationFollow();

protected:

    Vec3 deltaPos;
    BehaviorAgent* leader;
    float runSpeed;
    
    float epsilon;

    virtual void on_enter() override;
    virtual void on_update(float dt) override;
};