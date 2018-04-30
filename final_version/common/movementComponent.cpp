#include "movementComponent.h"

CMovementComponent::CMovementComponent(vec2 position, IBroadcaster* globalBroadcaster, IBroadcaster* localBroadcaster) : CManagerComponent(globalBroadcaster, localBroadcaster)
{
	m_velocity = vmake(CORE_FRand(-RAFT_SPEED, +RAFT_SPEED), CORE_FRand(-RAFT_SPEED, +RAFT_SPEED));
	m_position = position;
}

void CMovementComponent::updateLogic(float deltaTime) {
	// Try to move if the is no collision with other entities
	vec2 newPosition = vadd(m_position, vscale(m_velocity, deltaTime));
	CMessageGetCollision collisionMessage;
	collisionMessage.pointerToCreator = m_localBroadcaster;
	collisionMessage.position = newPosition;
	m_globalBroadcaster->broadcastMessage(&collisionMessage);
	CMessageGetSize sizeMessage;
	m_localBroadcaster->broadcastMessage(&sizeMessage);
	if (collisionMessage.collisionType == NONE || collisionMessage.collisionType == OVERLAP)
		m_position = newPosition;
	// If there is a collision, change my velocity accordingly
	else
	{
		if (collisionMessage.collisionType == HORIZONTAL_OPPOSITE || collisionMessage.collisionType == HORIZONTAL_FIRST)
			m_velocity = vmake(-m_velocity.x, m_velocity.y);
		else if (collisionMessage.collisionType == VERTICAL_OPPOSITE || collisionMessage.collisionType == VERTICAL_FIRST)
			m_velocity = vmake(m_velocity.x, -m_velocity.y);
		else if (collisionMessage.collisionType == DIAGONAL)
			m_velocity = vscale(m_velocity, -1.f);
		m_position = vadd(m_position, vscale(m_velocity, deltaTime));
	}
	// Notify other components of the position change
	CMessageChangePosition positionMessage;
	positionMessage.position = m_position;
	m_localBroadcaster->broadcastMessage(&positionMessage);
	// Rebound with screen margins
	if (m_velocity.x > 0.0)
	{
		if (m_position.x + sizeMessage.size.x > SCR_WIDTH)
			m_velocity.x *= -1.0;
	}
	else
		if (m_position.x < 0)
			m_velocity.x *= -1.0;
	if (m_velocity.y > 0.0)
	{
		if (m_position.y + sizeMessage.size.y > SCR_HEIGHT)
			m_velocity.y *= -1.0;
	}
	else
		if (m_position.y < 0)
			m_velocity.y *= -1.0;
}

void CMovementComponent::changePositionOnCollision(ECollision collisionType)
{
	if (collisionType == HORIZONTAL_OPPOSITE || collisionType == HORIZONTAL_SECOND)
		m_velocity = vmake(-m_velocity.x, m_velocity.y);
	else if (collisionType == VERTICAL_OPPOSITE || collisionType == VERTICAL_SECOND)
		m_velocity = vmake(m_velocity.x, -m_velocity.y);
	else if (collisionType == DIAGONAL)
		m_velocity = vscale(m_velocity, -1.f);
}

void CMovementComponent::manageMessage(CMessage* message)
{
	CManagerComponent::manageMessage(message);
	// Change the velocity of the entity
	CMessageChangeVelocity* setVelocityMessage = dynamic_cast<CMessageChangeVelocity*>(message);
	if (setVelocityMessage)
		m_velocity = setVelocityMessage->velocity;
	// Return the velocity of the entity
	CMessageGetVelocity* getVelocityMessage = dynamic_cast<CMessageGetVelocity*>(message);
	if (getVelocityMessage)
		if (!getVelocityMessage->consumed)
		{
			getVelocityMessage->consumed = true;
			getVelocityMessage->velocity = m_velocity;
		}
	// If other entity has collided with me, I get notified to change my velocity too
	CMessageCollisionNotifier* collisionMessage = dynamic_cast<CMessageCollisionNotifier*>(message);
	if (collisionMessage)
		changePositionOnCollision(collisionMessage->collisionType);
	// Return the position of the entity
	CMessageGetPosition* getPositionMessage = dynamic_cast<CMessageGetPosition*>(message);
	if (getPositionMessage)
		if (!getPositionMessage->consumed)
		{
			getPositionMessage->consumed = true;
			getPositionMessage->position = m_position;
		}
	// Change the position of the entity
	CMessageChangePosition* moveMessage = dynamic_cast<CMessageChangePosition*>(message);
	if (moveMessage)
		m_position = moveMessage->position;
	// Spawn a "destroyed" entity by changing its position until it does not collide with other entities
	CMessageSpawn* spawnMessage = dynamic_cast<CMessageSpawn*>(message);
	if (spawnMessage)
	{
		CMessageGetSize sizeMessage;
		m_localBroadcaster->broadcastMessage(&sizeMessage);
		ECollision collisionType = OVERLAP;
		while (collisionType != NONE)
		{
			CMessageActivateCollision childSpawnMessage;
			m_localBroadcaster->broadcastMessage(&childSpawnMessage);
			m_position = vmake(CORE_FRand(0.0, SCR_WIDTH - sizeMessage.size.x), CORE_FRand(0.0, SCR_HEIGHT - sizeMessage.size.y));
			CMessageGetCollision getCollisionMessage;
			getCollisionMessage.pointerToCreator = m_localBroadcaster;
			getCollisionMessage.position = m_position;
			m_globalBroadcaster->broadcastMessage(&getCollisionMessage);
			collisionType = getCollisionMessage.collisionType;
			CMessageDestroy childDestroyMessage;
			m_localBroadcaster->broadcastMessage(&childDestroyMessage);
		}

	}
}
