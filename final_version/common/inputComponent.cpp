#include "inputComponent.h"

CInputComponent::CInputComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	m_bIsEscapePressed = false;
	m_bIsEnterPressed = false;
}

void CInputComponent::updateInput(float deltaTime)
{
	// Check if the player is pressing a directional key
	CMessageChangeVelocity inputMessage;
	if (GetKeyState('W') & 0x8000)
		inputMessage.velocity = vadd(inputMessage.velocity, { 0.f, CHARACTER_SPEED });
	if (GetKeyState('S') & 0x8000)
		inputMessage.velocity = vadd(inputMessage.velocity, { 0.f, -CHARACTER_SPEED });
	if (GetKeyState('A') & 0x8000)
		inputMessage.velocity = vadd(inputMessage.velocity, { -CHARACTER_SPEED, 0.f });
	if (GetKeyState('D') & 0x8000)
		inputMessage.velocity = vadd(inputMessage.velocity, { CHARACTER_SPEED, 0.f });
	m_localBroadcaster->broadcastMessage(&inputMessage);
	// Check if the player has just paused the game
	if (!(GetKeyState(VK_ESCAPE) & 0x8000) & m_bIsEscapePressed)
		m_bIsEscapePressed = false;
	else if(GetKeyState(VK_ESCAPE) & 0x8000)
		if(!m_bIsEscapePressed)
		{
			m_bIsEscapePressed = true;
			CMessagePause pauseMessage;
			m_globalBroadcaster->broadcastMessage(&pauseMessage);
		}
	// Check if the player has just selected a menu option
	if (!(GetKeyState(VK_RETURN) & 0x8000) & m_bIsEnterPressed)
			m_bIsEnterPressed = false;
	else if (GetKeyState(VK_RETURN) & 0x8000)
		if (!m_bIsEnterPressed)
		{
			m_bIsEnterPressed = true;
			CMessageEnter enterMessage;
			m_localBroadcaster->broadcastMessage(&enterMessage);
		}
}
