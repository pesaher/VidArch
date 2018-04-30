#pragma once

#include "managerComponent.h"

class CHealthComponent: public CManagerComponent
{
private:
	float	m_health;
	float	m_dps;
	bool	m_bIsBeingDamaged;

	void updateLogic	(float deltaTime);
	void manageMessage	(CMessage* message);

public:
	CHealthComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, float health, float dps);
};
