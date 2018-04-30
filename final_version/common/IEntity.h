#pragma once

#include "base.h"
#include "IBroadcaster.h"

class IEntity : public IBroadcaster
{
protected:
	IEntity(vec2 position, vec2 size, IBroadcaster* world) : IBroadcaster() {}

	virtual void manageMessage(CMessage* message) = 0;

public:
	virtual ~IEntity() {}
	virtual void broadcastMessage(CMessage* message) = 0;
};