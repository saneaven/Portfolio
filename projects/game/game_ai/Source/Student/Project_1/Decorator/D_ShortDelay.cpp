#include <pch.h>
#include "D_ShortDelay.h"

D_ShortDelay::D_ShortDelay() : delay(0.0f)
{}

void D_ShortDelay::on_enter()
{
    delay = RNG::range(0.1f, 0.15f);

    BehaviorNode::on_enter();
}

void D_ShortDelay::on_update(float dt)
{
    delay -= dt;

    if (delay < 0.0f)
    {
        BehaviorNode* child = children.front();

        child->tick(dt);

        // assume same status as child
        set_status(child->get_status());
        set_result(child->get_result());
    }
}
