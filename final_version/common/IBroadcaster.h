#pragma once

#include "message.h"

class IBroadcaster
{
protected:
	IBroadcaster() {}
public:
	virtual ~IBroadcaster() {}

	virtual void broadcastMessage(CMessage* message) = 0;
};
