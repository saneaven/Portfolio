#pragma once

#include "pch.h"
#include "BehaviorNode.h"

class CircularFormatInfo
{
	bool isUpdated = false;
	float currentAngle = 0.f;
	float radius = 25.f;
	Vec3 centerPos = { 50., 0., 50. };

	float turnSpeed = PI/8;
	float radiusChangeSpeed = 0;

	BehaviorAgent* mainAgent = NULL;
public:
	CircularFormatInfo() = default;

	void SetAgent(BehaviorAgent* agent) { mainAgent = agent; }

	float GetAngle() { return currentAngle; }
	float GetRadius() { return radius; }
	void Update(float dt, BehaviorAgent* agent)
	{
		if (mainAgent == agent)
		{
			currentAngle += turnSpeed * dt;
			radius += radiusChangeSpeed * dt;
		}

	} // I feel so dumb but I cannot think better than this


	float GetTurnSpeed() { return turnSpeed; }
	void SetTurnSpeed(float speed, BehaviorAgent* agent) { if (mainAgent == agent) turnSpeed = speed; }

	Vec3 GetCenterPos() { return centerPos; }
	void GetCenterPos(Vec3 center, BehaviorAgent* agent) { if (mainAgent == agent) centerPos = center; }
};

CircularFormatInfo circularFormatManager; // A singleton format manager
