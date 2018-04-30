#include "menu.h"
#include "playerMenu.h"
#include "raft.h"
#include "renderComponent.h"
#include "scoreComponent.h"

CMenu::CMenu(GLuint backgroundTexture, vec2 backgroundSize, vec2 iceSize, vec2 raftSize, LanguageArray languageArray, uint32_t selectedLanguage) : ILevel(backgroundTexture, backgroundSize, iceSize, raftSize)
{
	// Set up the menu with fixed positions depending on the screen dimensions
	m_texts = languageArray;
	m_selectedLanguage = selectedLanguage;
	IBroadcaster* render = new CRenderComponent(backgroundTexture, true, false, this, this, backgroundSize, NO_PRIORITY);
	m_broadcasters.push_back(render);
	for (uint32_t i = 0; i < 3; ++i)
	{
		IBroadcaster* raft = new CRaft({ (SCR_WIDTH - raftSize.x) / 2.f, SCR_HEIGHT * (0.6f - 2.f * i / 9.f) - raftSize.y / 2.f }, raftSize, this);
		CMessageChangeVelocity velocityMessage;
		raft->broadcastMessage(&velocityMessage);
		CMessageChangeText textMessage;
		textMessage.counter = i;
		textMessage.texts = m_texts[m_selectedLanguage];
		raft->broadcastMessage(&textMessage);
		m_broadcasters.push_back(raft);
	}
	IBroadcaster* playerMenu = new CPlayerMenu(vmake(SCR_WIDTH / 2.f - 2.f * iceSize.x, (SCR_HEIGHT * 0.6f - iceSize.y / 2.f)), iceSize, this);
	m_broadcasters.push_back(playerMenu);
	GLuint logoTexture = CORE_LoadPNG("data/logo.png", true);
	IBroadcaster* logo = new CRenderComponent(logoTexture, false, true, this, this, { SCR_WIDTH * 0.9f, SCR_HEIGHT * 0.15f }, LOW);
	CMessageChangePosition positionMessage;
	positionMessage.position = { SCR_WIDTH * 0.05f, SCR_HEIGHT * 0.75f };
	logo->broadcastMessage(&positionMessage);
	m_broadcasters.push_back(logo);
}

CMenu::~CMenu()
{
	for (IBroadcaster* broadcaster : m_broadcasters)
		delete broadcaster;
}

void CMenu::update(float deltaTime)
{
	updateInput(deltaTime);
	updateLogic(deltaTime);
	updateRender(deltaTime);
}

void CMenu::updateInput(float deltaTime)
{
	CMessageUpdateInput inputMessage;
	inputMessage.deltaTime = deltaTime;
	broadcastMessage(&inputMessage);
}

void CMenu::updateLogic(float deltaTime)
{
	CMessageUpdateLogic logicMessage;
	logicMessage.deltaTime = deltaTime;
	broadcastMessage(&logicMessage);
}

void CMenu::updateRender(float deltaTime)
{
	CMessageUpdateRender renderMessage;
	renderMessage.deltaTime = deltaTime;
	for (int renderPriority = NO_PRIORITY; renderPriority <= TOP; ++renderPriority)
	{
		renderMessage.renderPriority = static_cast<EPriority>(renderPriority);
		broadcastMessage(&renderMessage);
	}
}

void CMenu::broadcastMessage(CMessage* message)
{
	manageMessage(message);
	for (IBroadcaster* broadcaster : m_broadcasters)
		broadcaster->broadcastMessage(message);
}

void CMenu::manageMessage(CMessage* message)
{
	// Return the current menu screen in which we are
	CMessageGetState* getStateMessage = dynamic_cast<CMessageGetState*>(message);
	if (getStateMessage)
		if (!getStateMessage->consumed)
		{
			getStateMessage->consumed = true;
			getStateMessage->state = m_state;
		}
	// Change the current menu screen in which we are
	CMessageChangeState* stateMessage = dynamic_cast<CMessageChangeState*>(message);
	if (stateMessage)
		if (!stateMessage->consumed)
		{
			stateMessage->consumed = true;
			CMessageGetPosition positionMessage;
			stateMessage->selector->broadcastMessage(&positionMessage);
			changeState(static_cast<int>(4.5f * (0.6f - (positionMessage.position.y + 50.f) / SCR_HEIGHT)) + 1);
		}
	// Update the menu, returning the current level and language to the levels manager
	CMessageUpdateWithReturn* updateMessage = dynamic_cast<CMessageUpdateWithReturn*>(message);
	if (updateMessage)
	{
		m_changeLevel = NONE_SELECTED;
		update(updateMessage->deltaTime);
		updateMessage->changeLevel = m_changeLevel;
		updateMessage->selectedLanguage = m_selectedLanguage;
	}
}

// Change between the menu screens, and start the game or exit if necessary
void CMenu::changeState(int selectedOption)
{
	CMessageChangeText textMessage;
	switch (m_state)
	{
	case MAIN:
		switch (selectedOption)
		{
		case FIRST:
			m_state = LEVEL;
			break;
		case SECOND:
			m_state = LANGUAGE;
			break;
		case THIRD:
			exit(0);
			break;
		default:
			m_state = MAIN;
			break;
		}
		break;
	case LEVEL:
		switch (selectedOption)
		{
		case FIRST:
			m_changeLevel = FIRST;
			break;
		case SECOND:
			m_changeLevel = SECOND;
			break;
		case THIRD:
		default:
			m_state = MAIN;
			break;
		}
		break;
	case LANGUAGE:
		switch (selectedOption)
		{
		case FIRST:
			m_selectedLanguage = 0;
			textMessage.texts = m_texts[m_selectedLanguage];
			broadcastMessage(&textMessage);
			m_state = MAIN;
			break;
		case SECOND:
			m_selectedLanguage = 1;
			textMessage.texts = m_texts[m_selectedLanguage];
			broadcastMessage(&textMessage);
			m_state = MAIN;
			break;
		case THIRD:
		default:
			m_state = MAIN;
			break;
		}
		break;
	default:
		break;
	}
}
