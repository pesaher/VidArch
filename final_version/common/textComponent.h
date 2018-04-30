#pragma once

#include "managerComponent.h"

class CTextComponent: public CManagerComponent
{
private:
	std::array<std::string, 3>	m_texts;
	EPriority					m_renderPriority;

	void updateRender	(float deltaTime, EPriority renderPriority);
	void manageMessage	(CMessage* message);

public:
	CTextComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, EPriority renderPriority = TOP);
};
