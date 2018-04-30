#include "renderComponent.h"

CRenderComponent::CRenderComponent(GLuint texture, bool bIsRecursive, bool bIsBlendEnabled, IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, vec2 size, EPriority renderPriority) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	m_texture = texture;
	m_bIsRecursive = bIsRecursive;
	m_bIsBlendEnabled = bIsBlendEnabled;
	m_size = size;
	m_position = { 0, 0 };
	m_renderPriority = renderPriority;
	m_bIsRenderable = true;
}

void CRenderComponent::updateRender(float deltaTime, EPriority renderPriority)
{
	// Render recursively (or not) if the selected priority is mine
	if(m_bIsRenderable)
		if (m_renderPriority == renderPriority)
		{
			if (m_bIsBlendEnabled)
				glEnable(GL_BLEND);
			if (m_bIsRecursive)
				for (uint32_t i = 0; i <= SCR_WIDTH / m_size.x; ++i)
					for (uint32_t j = 0; j <= SCR_HEIGHT / m_size.y; j++)
						CORE_RenderCenteredSprite(vmake(i * m_size.x + m_size.x / 2, j * m_size.y + m_size.y / 2), m_size, m_texture);
			else
			{
				CMessageGetDamage damageMessage;
				m_localBroadcaster->broadcastMessage(&damageMessage);
				// Change color if I'm being damaged
				if (damageMessage.bIsBeingDamaged)
					glColor4f(1.f, 0.f, 0.f, 1.f);
				CORE_RenderSprite(m_position, vadd(m_position, m_size), m_texture);
				if (damageMessage.bIsBeingDamaged)
					glColor4f(1.f, 1.f, 1.f, 1.f);
			}
			if (m_bIsBlendEnabled)
				glDisable(GL_BLEND);
		}
}

void CRenderComponent::manageMessage(CMessage* message)
{
	CManagerComponent::manageMessage(message);
	// Change the render position
	CMessageChangePosition* moveMessage = dynamic_cast<CMessageChangePosition*>(message);
	if (moveMessage)
		m_position = moveMessage->position;
	// Change the render size
	CMessageGetSize* sizeMessage = dynamic_cast<CMessageGetSize*>(message);
	if (sizeMessage)
		if (!sizeMessage->consumed)
		{
			sizeMessage->consumed = true;
			sizeMessage->size = m_size;
		}
	// Return the render position
	CMessageGetPosition* getPositionMessage = dynamic_cast<CMessageGetPosition*>(message);
	if (getPositionMessage)
		if (!getPositionMessage->consumed)
		{
			getPositionMessage->consumed = true;
			getPositionMessage->position = m_position;
		}
	// "Destroy" the entity (make it invisible)
	CMessageDestroy* messageDestroy = dynamic_cast<CMessageDestroy*>(message);
	if (messageDestroy)
		if(messageDestroy->objective == m_localBroadcaster)
			m_bIsRenderable = false;
	// Make the entity visible
	CMessageSpawn* messageSpawn = dynamic_cast<CMessageSpawn*>(message);
	if (messageSpawn)
		m_bIsRenderable = true;
}
