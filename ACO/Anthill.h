#pragma once
#include "Agent.h"
#include <unordered_map>
#include "Utility.h"

class Anthill : public Agent, public Interactive
{
public:
	//Parameters
	const int numberOfAnts = 1000;
	const float rho = 0.3f;
	const float a = 1.7f;
	const float b = 1.3f;
	const float pheromoneEpsilon = 0.0001f;

	Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr);
	~Anthill();

	void FindFood();
	HexData* GetNextField(std::vector<HexData*>& neighbors, const std::unordered_map<HexData*, HexData*>& visited, std::vector<std::pair<float, HexData*>>& possibleFields);

	void HandleKeyboard(sf::Keyboard::Key key) override;
	void HandleMouse(sf::Vector2f& mousePosition) override;
	void HandleMouse(sf::Mouse::Button mb) override;
	void Render(sf::RenderWindow* window) override;
	void Move() override;
	void DebugRender(sf::RenderWindow* window) override;

	void SpawnFood(const sf::Vector2i& pos);
	void DeleteFood(const sf::Vector2i& pos);

	int FindOptimalPath();

	std::unordered_map<std::pair<int, int>, Agent*, SimpleHash> foodSources;

	sf::Text debugText;
	sf::Font debugFont;
};
