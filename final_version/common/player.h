#pragma once

#include "entity.h"

class CPlayer : public CEntity
{
protected:
	virtual void manageMessage(CMessage* message);

public:
	CPlayer(vec2 position, vec2 size, IBroadcaster* globalBroadcaster);
	virtual ~CPlayer();
};
