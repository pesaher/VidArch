#include "healthComponent.h"

CHealthComponent::CHealthComponent(IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster, float health, float dps) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	m_health = health;
	m_dps = dps;
}

void CHealthComponent::updateLogic(float deltaTime)
{
	// Reduce the health if damage is being taken
	m_health -= m_dps * deltaTime * m_bIsBeingDamaged;
	if (m_health < 0.f)
	{
		// "Destroy" the entity if it has no health left
		CMessageDestroy destroyMessage;
		destroyMessage.objective = m_localBroadcaster;
		m_globalBroadcaster->broadcastMessage(&destroyMessage);
		// Try to "destroy" the player
		CMessageDestroyPlayer destroyPlayerMessage;
		m_localBroadcaster->broadcastMessage(&destroyPlayerMessage);
	}
}

void CHealthComponent::manageMessage(CMessage* message)
{
	CManagerComponent::manageMessage(message);
	// Change the damage to receive when updating
	CMessageDamage* setDamageMessage = dynamic_cast<CMessageDamage*>(message);
	if (setDamageMessage)
		m_bIsBeingDamaged = setDamageMessage->bIsBeingDamaged;
	// Return the damage to receive when updating
	CMessageGetDamage* getDamageMessage = dynamic_cast<CMessageGetDamage*>(message);
	if (getDamageMessage) {
		getDamageMessage->consumed = true;
		getDamageMessage->bIsBeingDamaged = m_bIsBeingDamaged;
	}
	// Return the current health of the entity
	CMessageSetHealth* healthMessage = dynamic_cast<CMessageSetHealth*>(message);
	if (healthMessage) {
		m_health = healthMessage->health;
	}
}
