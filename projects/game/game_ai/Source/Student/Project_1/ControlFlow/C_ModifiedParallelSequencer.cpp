#include "pch.h"
#include "C_ModifiedParallelSequencer.h"

void C_ModifiedParallelSequencer::ForceOtherFailed(BehaviorNode* success)
{
    for (auto&& child : children)
    {
        if (child != success)
        {
            child->on_failure();
        }
    }
}

void C_ModifiedParallelSequencer::on_update(float dt)
{
   // run all children
    // If One Child Failed, Other childs are pretended failed. to run on_end();

    bool allSuccess = true;

    for (auto&& child : children)
    {
        child->tick(dt);

        if (child->failed() == true)
        {
            ForceOtherFailed(child);
            on_failure();
            allSuccess = false;
        }
        else if (child->is_running() == true && child->succeeded() == false)
        {
            allSuccess = false;
        }
    }

    if (allSuccess == true)
    {
        on_success();
    }
}
