#pragma once
#include "Agent.h"
#include <unordered_map>
#include "Utility.h"

class Anthill : public Agent, public Interactive
{
public:
	const int numberOfAnts = 1000;

	Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr);
	~Anthill();

	void FindFood();
	HexData* GetNextField(std::vector<HexData*>& neighbors, const std::unordered_map<HexData*, HexData*>& visited);

	virtual void HandleKeyboard(sf::Keyboard::Key key);
	virtual void HandleMouse(sf::Vector2f& mousePosition);
	virtual void HandleMouse(sf::Mouse::Button mb);
	virtual void Render(sf::RenderWindow* window);
	virtual void Move() override;
	void DebugRender(sf::RenderWindow* window) override;

	void SpawnFood(const sf::Vector2i& pos);
	void DeleteFood(const sf::Vector2i& pos);

	int FindOptimalPath();

	std::unordered_map<std::pair<int, int>, Agent*, SimpleHash> foodSources;
	std::vector<HexData*> debugDraw;

	sf::Text debugText;
	sf::Font debugFont;
};
