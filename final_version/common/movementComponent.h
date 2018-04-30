#pragma once

#include "managerComponent.h"

class CMovementComponent : public CManagerComponent
{
private:
	vec2 m_velocity;
	vec2 m_position;

	void updateLogic				(float deltaTime);
	void changePositionOnCollision	(ECollision collisionType);
	void manageMessage				(CMessage* message);

public:
	CMovementComponent(vec2 position, IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster);
};
