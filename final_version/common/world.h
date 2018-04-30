#pragma once

#include "base.h"
#include "ILevel.h"

class CWorld : public ILevel
{
private:
	std::vector<IBroadcaster*>	m_broadcasters;
	bool						m_bIsPaused;
	ESelection					m_changeLevel;

	void updateInput	(float deltaTime);
	void updateLogic	(float deltaTime);
	void updateRender	(float deltaTime);
	void manageMessage	(CMessage* message);
	void pause			();
	void update			(float deltaTime);

public:
	CWorld(	uint32_t	numberOfRafts,
			GLuint		backgroundTexture,
			vec2		backgroundSize,
			vec2		iceSize,
			vec2		raftSize,
			vec2		fireSize);
	~CWorld();

	void broadcastMessage(CMessage* message);
};
