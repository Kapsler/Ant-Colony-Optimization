#include "Anthill.h"
#include <iostream>

Anthill::Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr) : Agent(filename, startingIndex, mapPtr)
{

}

Anthill::~Anthill()
{
}

void Anthill::FindFood()
{
	std::cout << "Finding Food" << std::endl;

	std::vector<HexData*> antPath;
	std::vector<HexData*> neighbors;
	sf::Vector2i antPos(positionIndex);

	for(int i = 0; i < numberOfAnts; ++i)
	{
		//startpoint
		antPath.push_back(map->GetHexDatByIndex(antPos.x, antPos.y));

		neighbors = map->GetNeighbors(map->GetHexDatByIndex(antPos.x, antPos.y), *map->GetMapPtr());


	

	}
}

HexData* Anthill::GetNextField(const std::vector<HexData*>& neighbors, const std::vector<HexData*>& visited)
{
	HexData* nextField = nullptr;

	//TODO Get possibilities and random selection




	return nextField;
}

void Anthill::HandleKeyboard(sf::Keyboard::Key key)
{
	if(key == sf::Keyboard::Key::M)
	{
		FindFood();
	}
}

void Anthill::HandleMouse(sf::Vector2f& mousePosition)
{

}

void Anthill::HandleMouse(sf::Mouse::Button mb)
{



}
