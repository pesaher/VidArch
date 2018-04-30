#pragma once

#include "managerComponent.h"

class CColliderComponent : public CManagerComponent
{
private:
	vec2 m_position;
	vec2 m_size;
	bool m_bIsCollisionable;

	void		manageMessage	(CMessage* message);
	ECollision	getCollisionType(IBroadcaster* remoteBroadcaster, vec2 otherPosition);

public:
	CColliderComponent(vec2 position, vec2 size, IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster);
};