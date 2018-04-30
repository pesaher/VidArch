#include "scoreComponent.h"

CScoreComponent::CScoreComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, EPriority renderPriority) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	m_score = 0;
	m_renderPriority = renderPriority;
}

int getDigits(long int number)
{
	int digits = 0;
	while (number != 0)
	{
		number /= 10;
		digits++;
	}
	return digits;
}

void CScoreComponent::updateLogic(float deltaTime)
{
	m_score += static_cast<long>(deltaTime * 1000.f);
}

void CScoreComponent::updateRender(float deltaTime, EPriority renderPriority)
{
	// Print the updated score if the selected priority is mine
	if (m_renderPriority == renderPriority)
	{
		glEnable(GL_BLEND);
		FONT_DrawString(vmake(static_cast<float>(SCR_WIDTH / 2 - getDigits(m_score) * 8), 16.f), std::to_string(static_cast<int>(ceil(m_score))).c_str());
		glDisable(GL_BLEND);
	}
}

void CScoreComponent::manageMessage(CMessage * message)
{
	CManagerComponent::manageMessage(message);
}
