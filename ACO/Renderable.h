#pragma once
#include <SFML/Graphics.hpp>

class Renderable
{
public:
	virtual void Render(sf::RenderWindow* window) = 0;
	virtual void DebugRender(sf::RenderWindow* window) = 0;
};
