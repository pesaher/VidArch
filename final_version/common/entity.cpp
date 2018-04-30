#include "entity.h"
#include "movementComponent.h"
#include "colliderComponent.h"

CEntity::CEntity(vec2 position, vec2 size, IBroadcaster* globalBroadcaster) : IEntity(position, size, globalBroadcaster)
{
	IBroadcaster* movement = new CMovementComponent(position, globalBroadcaster, this);
	m_childBroadcasters.push_back(movement);
	IBroadcaster* collider = new CColliderComponent(position, size, globalBroadcaster, this);
	m_childBroadcasters.push_back(collider);
	m_globalBroadcaster = globalBroadcaster;
}

void CEntity::manageMessage(CMessage* message)
{
	// Spawn an entity that has been "destroyed"
	CMessageSpawnConsumable* spawnMessageConsumable = dynamic_cast<CMessageSpawnConsumable*>(message);
	if(spawnMessageConsumable)
		if (!spawnMessageConsumable->consumed)
		{
			CMessageGetCollisionable collisionableMessage;
			for (IBroadcaster* childBroadcaster : m_childBroadcasters)
				childBroadcaster->broadcastMessage(&collisionableMessage);
			if (!collisionableMessage.bIsCollisionable)
			{
				spawnMessageConsumable->consumed = true;
				CMessageSpawn spawnMessage;
				for (IBroadcaster* childBroadcaster : m_childBroadcasters)
					childBroadcaster->broadcastMessage(&spawnMessage);
			}
		}
	// "Destroy" an entity (make it invisible and not collisionable)
	CMessageDestroy* destroyMessage = dynamic_cast<CMessageDestroy*>(message);
	if (destroyMessage)
		if (destroyMessage->objective == this)
		{
			CMessageSetHealth healthMessage;
			healthMessage.health = 100.f;
			broadcastMessage(&healthMessage);
			CMessageDamage damageMessage;
			damageMessage.bIsBeingDamaged = false;
			broadcastMessage(&damageMessage);
		}
}

CEntity::~CEntity()
{
	for (IBroadcaster* childBroadcaster : m_childBroadcasters)
		delete childBroadcaster;
}

void CEntity::broadcastMessage(CMessage* message)
{
	manageMessage(message);
	for (IBroadcaster* childBroadcaster : m_childBroadcasters)
		childBroadcaster->broadcastMessage(message);
}