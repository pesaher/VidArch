#pragma once

#include "entity.h"

class CFire : public CEntity
{
private:
	void manageMessage(CMessage* message);

public:
	CFire(vec2 position, vec2 size, IBroadcaster* globalBroadcaster);
	~CFire();
};
