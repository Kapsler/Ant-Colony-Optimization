#pragma once
#include "Agent.h"

class Anthill : public Agent, public Interactive
{
public:
	Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr);
	~Anthill();

	virtual void HandleKeyboard(sf::Keyboard::Key key);
	virtual void HandleMouse(sf::Vector2f& mousePosition);
	virtual void HandleMouse(sf::Mouse::Button mb);
};
