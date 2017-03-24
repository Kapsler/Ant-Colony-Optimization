#pragma once
#include "Agent.h"
#include <unordered_map>
#include "Utility.h"
#include "RNGesus.h"
#include <omp.h>

class Anthill : public Agent, public Interactive
{
public:
	//Parameters
	const int numberOfAnts = 1000;
	const float rho = 0.15f;
	const float a = 1.7f;
	const float b = 1.5f;
	const float pheromoneEpsilon = 0.01f;

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

	int OptimalPathHeuristic();
	void FindPheromonePaths(std::vector<HexData*>& out);

	std::unordered_map<std::pair<int, int>, Agent*, SimpleHash> foodSources;
	std::vector<HexData*> Astarpaths;

	sf::Text debugText;
	sf::Font debugFont;

	std::vector<RNGesus> geez;

	bool renderPheromoneFlag = false;
};
