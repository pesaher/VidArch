#include "world.h"
#include "player.h"
#include "raft.h"
#include "fire.h"
#include "renderComponent.h"
#include "scoreComponent.h"

CWorld::CWorld(uint32_t numberOfRafts, GLuint backgroundTexture, vec2 backgroundSize, vec2 iceSize, vec2 raftSize, vec2 fireSize) : ILevel(backgroundTexture, backgroundSize, iceSize, raftSize)
{
	IBroadcaster* render = new CRenderComponent(backgroundTexture, true, false, this, this, backgroundSize, NO_PRIORITY);
	m_broadcasters.push_back(render);
	// Spawn rafts only when they do not collide with anything
	for (uint32_t i = 0; i < numberOfRafts; ++i)
	{
		ECollision isColliding = OVERLAP;
		IBroadcaster* raft = nullptr;
		while (isColliding != NONE)
		{
			delete raft;
			vec2 newPosition = vmake(CORE_FRand(0.f, SCR_WIDTH - raftSize.x), CORE_FRand(0.f, SCR_HEIGHT - raftSize.y));
			raft = new CRaft(newPosition, raftSize, this);
			CMessageGetCollision collisionMessage;
			collisionMessage.pointerToCreator = raft;
			collisionMessage.position = newPosition;
			broadcastMessage(&collisionMessage);
			isColliding = collisionMessage.collisionType;
		}
		m_broadcasters.push_back(raft);
	}
	IBroadcaster* player = new CPlayer(vmake((SCR_WIDTH - iceSize.x) / 2.f, (SCR_HEIGHT - iceSize.y) / 2.f), iceSize, this);
	m_broadcasters.push_back(player);
	// Spawn the fire only when it does not collide with anything
	ECollision isColliding = OVERLAP;
	IBroadcaster* fire = nullptr;
	while (isColliding != NONE)
	{
		delete fire;
		vec2 newPosition = vmake(CORE_FRand(0.0, SCR_WIDTH - fireSize.x), CORE_FRand(0.0, SCR_HEIGHT - fireSize.y));
		fire = new CFire(newPosition, fireSize, this);
		CMessageGetCollision collisionMessage;
		collisionMessage.pointerToCreator = fire;
		collisionMessage.position = newPosition;
		broadcastMessage(&collisionMessage);
		isColliding = collisionMessage.collisionType;
	}
	m_broadcasters.push_back(fire);
	IBroadcaster* text = new CScoreComponent(this, this, TOP);
	m_broadcasters.push_back(text);
	m_bIsPaused = false;
}

CWorld::~CWorld()
{
	for (IBroadcaster* broadcaster : m_broadcasters)
		delete broadcaster;
}

void CWorld::update(float deltaTime)
{
	updateInput(deltaTime);
	updateLogic(deltaTime);
	updateRender(deltaTime);
}

void CWorld::updateInput(float deltaTime)
{
	CMessageUpdateInput inputMessage;
	inputMessage.deltaTime = deltaTime;
	broadcastMessage(&inputMessage);
}

void CWorld::updateLogic(float deltaTime)
{
	if (!m_bIsPaused)
	{
		// Randomly spawn "destroyed" entities
		if (rand() % 100 < 1)
		{
			CMessageSpawnConsumable spawnMessage;
			broadcastMessage(&spawnMessage);
		}
		CMessageUpdateLogic logicMessage;
		logicMessage.deltaTime = deltaTime;
		broadcastMessage(&logicMessage);
	}
}

void CWorld::updateRender(float deltaTime)
{
	CMessageUpdateRender renderMessage;
	renderMessage.deltaTime = deltaTime;
	for (int renderPriority = NO_PRIORITY; renderPriority <= TOP; ++renderPriority)
	{
		renderMessage.renderPriority = static_cast<EPriority>(renderPriority);
		broadcastMessage(&renderMessage);
	}
}

void CWorld::broadcastMessage(CMessage* message)
{
	manageMessage(message);
	for (IBroadcaster* broadcaster : m_broadcasters)
		broadcaster->broadcastMessage(message);
}

void CWorld::manageMessage(CMessage* message)
{
	// Pause the game
	CMessagePause* pauseMessage = dynamic_cast<CMessagePause*>(message);
	if (pauseMessage)
		pause();
	// Return to the main menu when the player is destroyed
	CMessageEndGame* endGameMessage = dynamic_cast<CMessageEndGame*>(message);
	if (endGameMessage)
	{
		m_changeLevel = THIRD;
	}
	// Update the game, and return the "return to the main menu" directive if it has been set
	CMessageUpdateWithReturn* updateMessage = dynamic_cast<CMessageUpdateWithReturn*>(message);
	if (updateMessage)
	{
		m_changeLevel = NONE_SELECTED;
		update(updateMessage->deltaTime);
		updateMessage->changeLevel = m_changeLevel;
	}

}

void CWorld::pause()
{
	m_bIsPaused = !m_bIsPaused;
}
