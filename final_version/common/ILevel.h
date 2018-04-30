#pragma once

#include "base.h"
#include "IBroadcaster.h"

class ILevel : public IBroadcaster
{
protected:
	virtual void updateInput	  (float deltaTime) = 0;
	virtual void updateLogic	  (float deltaTime) = 0;
	virtual void updateRender	  (float deltaTime) = 0;
	virtual void manageMessage	(CMessage* message) = 0;

public:
	ILevel(	GLuint		backgroundTexture,
			vec2		backgroundSize,
			vec2		iceSize,
			vec2		raftSize) {}
	virtual ~ILevel() {}

	virtual void update				(float deltaTime) = 0;
	virtual void broadcastMessage	(CMessage* message) = 0;
};
