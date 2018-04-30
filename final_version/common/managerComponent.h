#pragma once

#include "IManagerComponent.h"

class CManagerComponent : public IManagerComponent
{
protected:
	IBroadcaster* m_globalBroadcaster;
	IBroadcaster* m_localBroadcaster;

	CManagerComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster);

	virtual void	updateInput		(float deltaTime) {}
	virtual void	updateLogic		(float deltaTime) {}
	virtual void	updateRender	(float deltaTime, EPriority renderPriority) {}
	virtual void	manageMessage	(CMessage* message);

public:
	virtual void broadcastMessage(CMessage* message);
};