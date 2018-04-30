#include "textComponent.h"

CTextComponent::CTextComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, EPriority renderPriority) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	for(std::string text : m_texts)
		text = "";
	m_renderPriority = renderPriority;
}

void CTextComponent::updateRender(float deltaTime, EPriority renderPriority)
{
	if (m_renderPriority == renderPriority)
	{
		glEnable(GL_BLEND);
		CMessageGetPosition getPositionMessage;
		m_localBroadcaster->broadcastMessage(&getPositionMessage);
		CMessageGetSize getSizeMessage;
		m_localBroadcaster->broadcastMessage(&getSizeMessage);
		CMessageGetState getStateMessage;
		m_globalBroadcaster->broadcastMessage(&getStateMessage);
		FONT_DrawString(vmake(getPositionMessage.position.x + getSizeMessage.size.x / 2.f - m_texts[getStateMessage.state].size() * 8.f, getPositionMessage.position.y + getSizeMessage.size.y / 2.f - 8.f), m_texts[getStateMessage.state].c_str());
		glDisable(GL_BLEND);
	}
}

void CTextComponent::manageMessage(CMessage* message)
{
	CManagerComponent::manageMessage(message);
	// Change the text to print (depending on the current menu screen and localization)
	CMessageChangeText* textMessage = dynamic_cast<CMessageChangeText*>(message);
	if (textMessage)
		m_texts = textMessage->texts[textMessage->counter++];
}
