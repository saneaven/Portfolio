#pragma once
#include <memory>
#include <array>
#include "../Engine.h"
#include "Event.h"

namespace q_engine
{
	class EventManager
	{
	public:
		using IDType = int;
	private:
		static const int MAX_PENDING = 128;

		std::array<std::shared_ptr<IEvent>, MAX_PENDING> mEventArrays;
		IDType mBeginQueue{ 0 };
		IDType mEndQueue{ 0 };
	public:
		template <typename T, typename... Args>
		void AddEvent(Args... args); // you should add proper component separately

		void ResolveEvents();
	};
}
