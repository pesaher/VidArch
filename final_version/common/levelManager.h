#pragma once

#include "ILevelManager.h"
#include "base.h"

class IBroadcaster;

class CLevelManager : public ILevelManager
{
private:
	IBroadcaster*	m_broadcaster;
	LanguageArray	m_languageArray;
	uint32_t		m_selectedLanguage;

public:
	CLevelManager();
	~CLevelManager();

	void update (float deltaTime);
};
