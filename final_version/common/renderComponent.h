#pragma once

#include "managerComponent.h"

class CRenderComponent: public CManagerComponent
{
private:
	GLuint		m_texture;
	bool		m_bIsRecursive;
	bool		m_bIsBlendEnabled;
	vec2		m_size;
	vec2		m_position;
	EPriority	m_renderPriority;
	bool		m_bIsRenderable;

	void updateRender	(float deltaTime, EPriority renderPriority);
	void manageMessage	(CMessage* message);

public:
	CRenderComponent(GLuint texture, bool bIsRecursive, bool bIsBlendEnabled, IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, vec2 size, EPriority renderPriority);
};
