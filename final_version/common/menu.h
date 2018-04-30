#pragma once

#include "base.h"
#include "ILevel.h"

class CMenu : public ILevel
{
private:
	std::vector<IBroadcaster*>	m_broadcasters;
	LanguageArray				m_texts;
	EMenu						m_state;
	ESelection					m_changeLevel;
	uint32_t					m_selectedLanguage;

	void updateInput	(float deltaTime);
	void updateLogic	(float deltaTime);
	void updateRender	(float deltaTime);
	void manageMessage	(CMessage* message);
	void changeState	(int selectedOption);

public:
	CMenu(	GLuint			backgroundTexture,
			vec2			backgroundSize,
			vec2			iceSize,
			vec2			raftSize,
			LanguageArray	languageArray,
			uint32_t		selectedLanguage);
	~CMenu();

	void update				(float deltaTime);
	void broadcastMessage	(CMessage* message);
};
