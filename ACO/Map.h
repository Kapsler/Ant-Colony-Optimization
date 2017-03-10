#pragma once
#include <vector>
#include "HexData.h"
#include "Renderable.h"
#include "Interactive.h"
#include <deque>
#include "ThreatStencil.h"

class Agent;

class Map : public Renderable, public Interactive
{
public:
	explicit Map(float screenWidth, float screenHeight, const std::string& filename);
	virtual ~Map();

	void Render(sf::RenderWindow* window) override;
	void DebugRender(sf::RenderWindow* window) override;
	void HandleKeyboard(sf::Keyboard::Key key) override;
	void HandleMouse(sf::Mouse::Button mb) override;
	void HandleMouse(sf::Vector2f& mousePosition) override;
	std::vector<std::vector<HexData*>>* GetMapPtr();
	HexData* GetSelectedHex() const;
	float GetHexSize() const;
	sf::Vector2f GetPositionByIndex(int x, int y);
	sf::Vector2f GetPositionByIndex(sf::Vector2i posIndex);
	HexData* GetHexDatByIndex(int x, int y);
	void AddThreat(Agent* threat);
	void ResetThreat(Agent* toIgnore);
	std::vector<HexData*> AStarPath(HexData* start, HexData* finish, std::vector<std::vector<HexData*>> &usedMap, Agent* toIgnore);

private:
	void GenerateFromImage(float screenWidth, float screenHeight, const sf::Image& mapImage);
	static float distanceBetweenFloatPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);
	void SetCurrentHex(const sf::Vector2f& mousePos);
	void LoadMapFromImage(float screenWidth, float screenHeight, const std::string& filename);
	void DebugRenderIndices(sf::RenderWindow *window);
	void DebugRenderThreat(sf::RenderWindow *window);
	void DebugRenderDifficulty(sf::RenderWindow *window);

	std::vector<HexData*> GetNeighbors(HexData* current, std::vector<std::vector<HexData*>> &usedMap);
	int GetDifficulty(HexData* HexToTest);

	std::vector<std::vector<HexData*>> hexMap;
	HexData* selectedHexDat = nullptr;

	int rows, columns;
	float hexsize; 

	std::vector<Agent*> threats;

	sf::Color waterColor = sf::Color(60, 180, 255);
	sf::Color grassColor = sf::Color(50, 150, 60);
	sf::Color earthColor = sf::Color(160, 100, 60);
	sf::Color sandColor = sf::Color(200, 180, 70);
	sf::Color streetColor = sf::Color(130, 130, 130);
	sf::Color notfoundColor = sf::Color::Magenta;

	static const int unpassable = 1000;
	int notfoundDifficulty = 1000;
	int waterDifficulty = 1000;
	int grassDifficulty = 3;
	int earthDifficulty = 1;
	int sandDifficulty = 5;
	int streetDifficulty = 10;

	sf::Text debugText;
	sf::Font debugFont;
	std::vector<HexData*> debugPath;

	bool difficultyRenderFlag = false;
	bool indicesRenderFlag = false;
	bool threatRenderFlag = false;
};
