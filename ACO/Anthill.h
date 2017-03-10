#pragma once
#include "Agent.h"

class Anthill : public Agent, public Interactive
{
public:
	const int numberOfAnts = 1;

	Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr);
	~Anthill();

	void FindFood();
	HexData* GetNextField(const std::vector<HexData*>& neighbors, const std::vector<HexData*>& visited);

	virtual void HandleKeyboard(sf::Keyboard::Key key);
	virtual void HandleMouse(sf::Vector2f& mousePosition);
	virtual void HandleMouse(sf::Mouse::Button mb);
};
