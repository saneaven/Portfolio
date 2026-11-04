#pragma once

#include "EventManager.h"

namespace q_engine
{
	template <typename T, typename... Args>
	void EventManager::AddEvent(Args... args)
	{
		static_assert(std::is_base_of_v<Event<T>, T>, "T is not Event when Add Event");

		mEndQueue++;

		if (mEndQueue >= MAX_PENDING)
		{
			//TODO Call logger to queue number reset to 0
			mEndQueue = 0;
		}

		if (mEndQueue == mBeginQueue)
		{
			//TODO Call logger error (interrupt queue)
			mBeginQueue++;
			if (mBeginQueue >= MAX_PENDING)
			{
				//TODO Call logger to begin queue number reset to 0
				mBeginQueue = 0;
			}
		}

		mEventArrays[mEndQueue] = std::make_shared<T>(args...);
	}

	void EventManager::ResolveEvents()
	{
		for (int i = mBeginQueue; i != mEndQueue; ++i)
		{
			if (i >= MAX_PENDING)
			{
				i = -1;
				continue;
			}
			mEventArrays[i]->ResolveEvent();
		}

		mBeginQueue = mEndQueue;
	}
}