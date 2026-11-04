/*
	File Name: Event.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2020 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include "../ecs/IDGenerator.h"

namespace q_engine
{
	class IEvent
	{
	protected:
		virtual void ResolveEvent() = 0;
	public:
		virtual ~IEvent() = default;

		friend class EventManager;
	};

	template <typename T>
	class Event : public IEvent
	{
		inline static int typeID = IDGenerator::GenerateTypeID();

		friend class EventManager;
	};
}