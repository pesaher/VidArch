#include "fire.h"
#include "renderComponent.h"
#include "healthComponent.h"

CFire::CFire(vec2 position, vec2 size, IBroadcaster* globalBroadcaster) : CEntity(position, size, globalBroadcaster)
{
	GLuint texture = CORE_LoadPNG("data/fire.png", false);
	IBroadcaster* render = new CRenderComponent(texture, false, true, globalBroadcaster, this, size, MEDIUM);
	m_childBroadcasters.push_back(render);
	IBroadcaster* health = new CHealthComponent(globalBroadcaster, this, 100.f, 100000.f);
	m_childBroadcasters.push_back(health);
}

CFire::~CFire()
{
	CEntity::~CEntity();
}

void CFire::manageMessage(CMessage* message)
{
	CEntity::manageMessage(message);
	// Notify other entities if they are touching the fire
	CMessageUpdateLogic* logicMessage = dynamic_cast<CMessageUpdateLogic*>(message);
	if (logicMessage)
	{
		CMessageGetPosition positionMessage;
		broadcastMessage(&positionMessage);
		CMessageGetCollision collisionMessage;
		collisionMessage.pointerToCreator = this;
		collisionMessage.position = positionMessage.position;
		m_globalBroadcaster->broadcastMessage(&collisionMessage);
		if (collisionMessage.collisionType == OVERLAP)
		{
			CMessageDamage damageMessage;
			damageMessage.bIsBeingDamaged = true;
			broadcastMessage(&damageMessage);
			collisionMessage.otherCollider->broadcastMessage(&damageMessage);
		}
	}
}