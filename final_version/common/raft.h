#pragma once

#include "entity.h"

class CRaft : public CEntity
{
private:
	void manageMessage(CMessage* message);

public:
	CRaft(vec2 position, vec2 size, IBroadcaster* globalBroadcaster);
	~CRaft();
};