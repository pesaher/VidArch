#pragma once

#include "managerComponent.h"

class CScoreComponent: public CManagerComponent
{
private:
	long int	m_score;
	EPriority	m_renderPriority;

	void updateLogic	(float deltaTime);
	void updateRender	(float deltaTime, EPriority renderPriority);
	void manageMessage	(CMessage* message);

public:
	CScoreComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, EPriority renderPriority = TOP);
};
