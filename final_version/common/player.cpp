#include "player.h"
#include "inputComponent.h"
#include "renderComponent.h"
#include "healthComponent.h"

CPlayer::CPlayer(vec2 position, vec2 size, IBroadcaster* globalBroadcaster) : CEntity(position, size, globalBroadcaster)
{
	IBroadcaster* input = new CInputComponent(globalBroadcaster, this);
	m_childBroadcasters.push_back(input);
	GLuint texture = CORE_LoadPNG("data/ice_cube.png", false);
	IBroadcaster* render = new CRenderComponent(texture, false, true, globalBroadcaster, this, size, HIGH);
	m_childBroadcasters.push_back(render);
	IBroadcaster* health = new CHealthComponent(globalBroadcaster, this, 100.f, 50.f);
	m_childBroadcasters.push_back(health);
}

CPlayer::~CPlayer()
{
	CEntity::~CEntity();
}

void CPlayer::manageMessage(CMessage* message)
{
	CEntity::manageMessage(message);
	CMessageUpdateLogic* logicMessage = dynamic_cast<CMessageUpdateLogic*>(message);
	if (logicMessage)
	{
		// Get damaged if the ice is not colliding with anything
		CMessageGetPosition positionMessage;
		broadcastMessage(&positionMessage);
		CMessageGetCollision collisionMessage;
		collisionMessage.pointerToCreator = this;
		collisionMessage.position = positionMessage.position;
		m_globalBroadcaster->broadcastMessage(&collisionMessage);
		CMessageDamage damageMessage;
		if (collisionMessage.collisionType == NONE)
			damageMessage.bIsBeingDamaged = true;
		broadcastMessage(&damageMessage);
	}
	// Return to the main menu if the player has been destroyed
	CMessageDestroyPlayer* destroyPlayerMessage = dynamic_cast<CMessageDestroyPlayer*>(message);
	if (destroyPlayerMessage)
	{
		CMessageEndGame endGameMessage;
		m_globalBroadcaster->broadcastMessage(&endGameMessage);
	}
}
