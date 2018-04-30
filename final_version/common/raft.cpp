#include "raft.h"
#include "renderComponent.h"
#include "healthComponent.h"
#include "textComponent.h"

CRaft::CRaft(vec2 position, vec2 size, IBroadcaster* globalBroadcaster) : CEntity(position, size, globalBroadcaster)
{
	GLuint texture = CORE_LoadPNG("data/raft.png", false);
	IBroadcaster* render = new CRenderComponent(texture, false, false, globalBroadcaster, this, size, LOW);
	m_childBroadcasters.push_back(render);
	IBroadcaster* health = new CHealthComponent(globalBroadcaster, this, 100.f, 20.f);
	m_childBroadcasters.push_back(health);
	IBroadcaster* text = new CTextComponent(globalBroadcaster, this, MEDIUM);
	m_childBroadcasters.push_back(text);
}

CRaft::~CRaft()
{
	CEntity::~CEntity();
}

void CRaft::manageMessage(CMessage* message)
{
	CEntity::manageMessage(message);
}