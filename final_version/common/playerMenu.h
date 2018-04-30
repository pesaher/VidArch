#pragma once

#include "player.h"

class CPlayerMenu : public CPlayer
{
private:
	void manageMessage(CMessage* message);

public:
	CPlayerMenu(vec2 position, vec2 size, IBroadcaster* globalBroadcaster);
	~CPlayerMenu();
};
