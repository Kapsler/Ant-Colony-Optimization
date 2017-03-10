#pragma once
#include "Hexagon.h"

struct HexData
{
	Hexagon* hex;
	sf::Vector2i index;
	int terrain;
	int threat;
	float pheromones;
	bool isFood;
};
