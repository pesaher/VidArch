#pragma once

#include "IEntity.h"

class IManagerComponent;

class CEntity : public IEntity
{
protected:
	IBroadcaster*					m_globalBroadcaster;
	std::vector<IBroadcaster*>		m_childBroadcasters;

	CEntity(vec2 position, vec2 size, IBroadcaster* globalBroadcaster);

	virtual void manageMessage(CMessage* message);

public:
	virtual			~CEntity();
	virtual void	broadcastMessage(CMessage* message);
};