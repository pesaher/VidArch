#include "playerMenu.h"
#include "inputComponent.h"
#include "renderComponent.h"
#include "healthComponent.h"

CPlayerMenu::CPlayerMenu(vec2 position, vec2 size, IBroadcaster* globalBroadcaster) : CPlayer(position, size, globalBroadcaster)
{

}

CPlayerMenu::~CPlayerMenu()
{
	CEntity::~CEntity();
}

void CPlayerMenu::manageMessage(CMessage* message)
{
	CPlayer::manageMessage(message);
	CMessageEnter* enterMessage = dynamic_cast<CMessageEnter*>(message);
	if (enterMessage)
	{
		// Change the menu option where the ice is located
		CMessageGetCollision collisionMessage;
		collisionMessage.pointerToCreator = this;
		CMessageGetPosition positionMessage;
		for (IBroadcaster* broadcaster : m_childBroadcasters)
			broadcaster->broadcastMessage(&positionMessage);
		collisionMessage.position = positionMessage.position;
		m_globalBroadcaster->broadcastMessage(&collisionMessage);
		if (collisionMessage.collisionType == OVERLAP)
		{
			CMessageChangeState changeStateMessage;
			changeStateMessage.selector = collisionMessage.otherCollider;
			m_globalBroadcaster->broadcastMessage(&changeStateMessage);
		}
	}
	// Make the ice cube in the menu invincible (set DPS to 0)
	CMessageDamage damageMessage;
	for (IBroadcaster* broadcaster : m_childBroadcasters)
		broadcaster->broadcastMessage(&damageMessage);
}
