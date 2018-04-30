#pragma once

class ILevelManager
{
public:
	ILevelManager() {}
	virtual ~ILevelManager() {}

	virtual void update(float deltaTime) = 0;
};
