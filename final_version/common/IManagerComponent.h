#pragma once

#include "IBroadcaster.h"

class IManagerComponent : public IBroadcaster
{
protected:
	IManagerComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster) : IBroadcaster() {}

	virtual void updateInput	(float deltaTime)							= 0;
	virtual void updateLogic	(float deltaTime)							= 0;
	virtual void updateRender	(float deltaTime, EPriority renderPriority)	= 0;
	virtual void manageMessage	(CMessage* message)							= 0;

public:
	virtual void broadcastMessage(CMessage* message) = 0;
};