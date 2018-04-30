#include "colliderComponent.h"
#include "raft.h"

CColliderComponent::CColliderComponent(vec2 position, vec2 size, IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	m_position = position;
	m_size = size;
	m_bIsCollisionable = true;
}

// Check if two objects are overlapping
bool getIsOverlapping(float a0, float a1, float b0, float b1)
{
	bool overlapResult = false;
	if (a0 <= b0)
	{
		if (a1 >= b0)
			overlapResult = true;
	}
	else if (b1 >= a0)
		overlapResult = true;
	return overlapResult;
}

// Check if two objects are colliding
ECollision CColliderComponent::getCollisionType(IBroadcaster* remoteBroadcaster, vec2 otherPosition)
{
	ECollision overlapResult = NONE;
	if (m_bIsCollisionable)
	{
		// Check if the other entity is collisionable
		CMessageGetCollisionable getIsCollisionable;
		remoteBroadcaster->broadcastMessage(&getIsCollisionable);
		if (getIsCollisionable.bIsCollisionable)
		{
			// Check the collision type depending on the positions, sizes and velocities
			CMessageGetSize sizeMessage2;
			remoteBroadcaster->broadcastMessage(&sizeMessage2);
			if (getIsOverlapping(m_position.x, m_position.x + m_size.x, otherPosition.x, otherPosition.x + sizeMessage2.size.x)
				&&
				getIsOverlapping(m_position.y, m_position.y + m_size.y, otherPosition.y, otherPosition.y + sizeMessage2.size.y)) {
				float horizontalDistance = abs(abs(m_position.x - otherPosition.x) - m_size.x);
				float verticalDistance = abs(abs(m_position.y - otherPosition.y) - m_size.y);
				CRaft* bIsRaftLocal = dynamic_cast<CRaft*>(m_localBroadcaster);
				CRaft* bIsRaftRemote = dynamic_cast<CRaft*>(remoteBroadcaster);
				if (!bIsRaftLocal || !bIsRaftRemote)
					overlapResult = OVERLAP;
				else
				{
					CMessageGetVelocity velocityMessage1;
					m_localBroadcaster->broadcastMessage(&velocityMessage1);
					CMessageGetVelocity velocityMessage2;
					remoteBroadcaster->broadcastMessage(&velocityMessage2);
					if (verticalDistance > horizontalDistance)
					{
						if (velocityMessage1.velocity.x > 0 && velocityMessage2.velocity.x < 0 || velocityMessage1.velocity.x < 0 && velocityMessage2.velocity.x > 0)
							overlapResult = HORIZONTAL_OPPOSITE;
						else if (abs(velocityMessage1.velocity.x) > abs(velocityMessage2.velocity.x))
							overlapResult = HORIZONTAL_FIRST;
						else
							overlapResult = HORIZONTAL_SECOND;
					}
					else if (verticalDistance < horizontalDistance)
					{
						if (velocityMessage1.velocity.y > 0 && velocityMessage2.velocity.y < 0 || velocityMessage1.velocity.y < 0 && velocityMessage2.velocity.y > 0)
							overlapResult = VERTICAL_OPPOSITE;
						else if (abs(velocityMessage1.velocity.y) > abs(velocityMessage2.velocity.y))
							overlapResult = VERTICAL_FIRST;
						else
							overlapResult = VERTICAL_SECOND;
					}
					else
						overlapResult = DIAGONAL;
				}
			}
			CMessageCollisionNotifier collisionMessage;
			collisionMessage.collisionType = overlapResult;
			m_localBroadcaster->broadcastMessage(&collisionMessage);
		}
	}
	return overlapResult;
}

void CColliderComponent::manageMessage(CMessage* message)
{
	CManagerComponent::manageMessage(message);
	// Return the collider size
	CMessageGetSize* sizeMessage = dynamic_cast<CMessageGetSize*>(message);
	if (sizeMessage)
		if (!sizeMessage->consumed)
		{
			sizeMessage->consumed = true;
			sizeMessage->size = m_size;
		}
	// Check if two objects are colliding
	CMessageGetCollision* collisionMessage = dynamic_cast<CMessageGetCollision*>(message);
	if (collisionMessage)
		if(collisionMessage->pointerToCreator != m_localBroadcaster)
			if (!collisionMessage->consumed)
			{
				ECollision collisionResult = getCollisionType(collisionMessage->pointerToCreator, collisionMessage->position);
				if (collisionResult != NONE && collisionResult != OVERLAP)
					collisionMessage->consumed = true;
				else
				{
					CRaft* bIsRaftLocal = dynamic_cast<CRaft*>(m_localBroadcaster);
					CRaft* bIsRaftRemote = dynamic_cast<CRaft*>(collisionMessage->pointerToCreator);
					if (bIsRaftLocal && !bIsRaftRemote)
						if (collisionResult == OVERLAP)
							collisionMessage->consumed = true;
				}
				collisionMessage->collisionType = collisionResult;
				collisionMessage->otherCollider = m_localBroadcaster;
			}
	// Change the collider position
	CMessageChangePosition* moveMessage = dynamic_cast<CMessageChangePosition*>(message);
	if (moveMessage)
		m_position = moveMessage->position;
	// Return the collider position
	CMessageGetPosition* getPositionMessage = dynamic_cast<CMessageGetPosition*>(message);
	if (getPositionMessage)
		if (!getPositionMessage->consumed)
		{
			getPositionMessage->consumed = true;
			getPositionMessage->position = m_position;
		}
	// "Destroy" the entity (make it not collisionable)
	CMessageDestroy* messageDestroy = dynamic_cast<CMessageDestroy*>(message);
	if (messageDestroy)
		if (messageDestroy->objective == m_localBroadcaster)
			m_bIsCollisionable = false;
	// Make the entity collisionable (other components read this message too)
	CMessageSpawn* messageSpawn = dynamic_cast<CMessageSpawn*>(message);
	if (messageSpawn)
		m_bIsCollisionable = true;
	// Check if the entity is collisionable
	CMessageGetCollisionable* getIsCollisionable = dynamic_cast<CMessageGetCollisionable*>(message);
	if (getIsCollisionable)
		if (!getIsCollisionable->consumed)
		{
			getIsCollisionable->consumed = true;
			getIsCollisionable->bIsCollisionable = m_bIsCollisionable;
		}
	// Make the entity collisionable (only this component reads this message)
	CMessageActivateCollision* messageActivateCollision = dynamic_cast<CMessageActivateCollision*>(message);
	if (messageActivateCollision)
		m_bIsCollisionable = true;
}