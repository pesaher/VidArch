#pragma once

#include "managerComponent.h"

class CInputComponent : public CManagerComponent
{
private:
	bool m_bIsEscapePressed;
	bool m_bIsEnterPressed;

public:
	CInputComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster);

	void updateInput(float deltaTime);
};
