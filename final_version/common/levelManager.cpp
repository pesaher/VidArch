#include "levelManager.h"
#include "menu.h"
#include "world.h"
#include "json.hpp"
#include <fstream>

struct CLevelData
{
	uint32_t numberOfRafts = 0;
	std::string filename = "";
};

CLevelData loadLevel(const char* filename, ESelection level);
LanguageArray loadLanguage(const char* filename);

CLevelManager::CLevelManager()
{
	FONT_Init();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClearColor(0.0f, 0.1f, 0.3f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLuint backgroundTexture = CORE_LoadPNG("data/ice.png", true);
	m_languageArray = loadLanguage("data/language.json");
	m_selectedLanguage = 0;
	// Create a menu on startup
	m_broadcaster = new CMenu(backgroundTexture, { 256.f, 256.f }, { 50.f, 50.f }, { 200.f, 100.f }, m_languageArray, m_selectedLanguage);
}

CLevelManager::~CLevelManager()
{
	delete m_broadcaster;
}

void CLevelManager::update(float deltaTime)
{
	// Update the current level and check if there was a language or level change
	CMessageUpdateWithReturn updateMessage;
	updateMessage.deltaTime = deltaTime;
	glClear(GL_COLOR_BUFFER_BIT);
	m_broadcaster->broadcastMessage(&updateMessage);
	if (updateMessage.selectedLanguage != 2)
		m_selectedLanguage = updateMessage.selectedLanguage;
	GLuint backgroundTexture;
	CLevelData levelData;
	switch (updateMessage.changeLevel)
	{
	case FIRST:
		delete m_broadcaster;
		levelData = loadLevel("data/levels.json", FIRST);
		backgroundTexture = CORE_LoadPNG(levelData.filename.c_str(), true);
		m_broadcaster = new CWorld(levelData.numberOfRafts, backgroundTexture, { 256.f, 256.f }, { 50.f, 50.f }, { 200.f, 100.f }, { 75.f, 75.f });
		break;
	case SECOND:
		delete m_broadcaster;
		levelData = loadLevel("data/levels.json", SECOND);
		backgroundTexture = CORE_LoadPNG(levelData.filename.c_str(), true);
		m_broadcaster = new CWorld(levelData.numberOfRafts, backgroundTexture, { 256.f, 256.f }, { 50.f, 50.f }, { 200.f, 100.f }, { 75.f, 75.f });
		break;
	case THIRD:
		delete m_broadcaster;
		backgroundTexture = CORE_LoadPNG("data/ice.png", true);
		m_broadcaster = new CMenu(backgroundTexture, { 256.f, 256.f }, { 50.f, 50.f }, { 200.f, 100.f }, m_languageArray, m_selectedLanguage);
		break;
	default:
		break;
	}
}

// Load the level details from a JSON file
CLevelData loadLevel(const char* filename, ESelection level)
{
	using json = nlohmann::json;

	json file;
	std::ifstream ifs(filename);
	ifs >> file;
	int counter = ESelection::FIRST;
	CLevelData levelData;
	for (json::iterator it = file.begin(); it != file.end(); ++it)
	{
		if (counter == level)
		{
			levelData.numberOfRafts = (*it).find("number_of_rafts")->get<uint32_t>();
			levelData.filename = (*it).find("background")->get<std::string>();
			break;
		}
		counter += 1;
	}
	return levelData;
}

// Load the localization details from a JSON file
LanguageArray loadLanguage(const char* filename)
{
	using json = nlohmann::json;

	json file;
	std::ifstream ifs(filename);
	ifs >> file;
	int counter = 0;
	LanguageArray languageArray;
	for (json::iterator it = file.begin(); it != file.end(); ++it)
	{
		languageArray[counter][0][0] = (*it).find("play")->get<std::string>();
		languageArray[counter][0][1] = (*it).find("easy")->get<std::string>();
		languageArray[counter][0][2] = (*it).find("english")->get<std::string>();
		languageArray[counter][1][0] = (*it).find("languages")->get<std::string>();
		languageArray[counter][1][1] = (*it).find("hard")->get<std::string>();
		languageArray[counter][1][2] = (*it).find("spanish")->get<std::string>();
		languageArray[counter][2][0] = (*it).find("exit")->get<std::string>();
		languageArray[counter][2][1] = (*it).find("back")->get<std::string>();
		languageArray[counter][2][2] = (*it).find("back")->get<std::string>();
		counter += 1;
	}
	return languageArray;
}
