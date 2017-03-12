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
	bool done = false;

	//startpoint
	antPath.push_back(map->GetHexDatByIndex(antPos.x, antPos.y));

	for(int i = 0; i < numberOfAnts; ++i)
	{
		//Traverse - Begin
		neighbors = map->GetNeighbors(map->GetHexDatByIndex(antPos.x, antPos.y), *map->GetMapPtr());

		while(!done)
		{
			GetNextField(neighbors, antPath);
			done = true;
		}

		//Traverse - End
	}
}

HexData* Anthill::GetNextField(const std::vector<HexData*>& neighbors, const std::vector<HexData*>& visited)
{
	HexData* nextField = nullptr;
	std::vector<std::pair<float, HexData*>> possibleFields;

	std::cout << "Neighbors: " << neighbors.size() << std::endl;
	std::cout << "Visited: " << visited.size() << std::endl;

	//Find scaling factor
	float scalingFactor = 0;

	for (const auto& n : neighbors)
	{
		if (std::find(visited.begin(), visited.end(), n) == visited.end() && n->terrain < map->unpassable)
		{
			float pher = n->pheromones;

			if (pher == 0)
			{
				pher = 0.00001f;
			}

			scalingFactor += (pher * n->terrain);

		}
	}

	//get possibilities per field
	for(const auto& n : neighbors)
	{
		if(std::find(visited.begin(), visited.end(), n) == visited.end() && n->terrain < map->unpassable)
		{
			float p = 0;
			float pher = n->pheromones;

			if (pher == 0)
			{
				pher = 0.00001f;
			}

			p = (pher * n->terrain) / (scalingFactor);

			possibleFields.push_back({ p, n });
		}
	}

	//Choose random field by possibility
	float randVal = (static_cast<double>(rand()) / (RAND_MAX));

	std::cout << randVal << std::endl;

	for(const auto& f : possibleFields)
	{
		std::cout << "Field: " << f.second->index.x << " " << f.second->index.y << " Possibility: " << f.first << std::endl;
		if(randVal < f.first)
		{
			std::cout << "Next field is " << f.second->index.x << " " << f.second->index.y << std::endl;
			nextField = f.second;
			break;
		} 

		randVal -= f.first;
		
	}

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
	if(mb == sf::Mouse::Button::Left)
	{
		sf::Vector2i pos = map->GetSelectedHex()->index;

		if (foodSources.find({ pos.x, pos.y }) != foodSources.end())
		{
			DeleteFood(pos);
		} else
		{
			SpawnFood(pos);
		}
	}
}

void Anthill::Render(sf::RenderWindow* window)
{
	Agent::Render(window);

	//Render Food
	for(auto it = foodSources.begin(); it != foodSources.end(); ++it)
	{
		it->second->Render(window);
	}
}

void Anthill::SpawnFood(const sf::Vector2i& pos)
{ 
	std::cout << "Spawning Food at " << pos.x << " " << pos.y << std::endl;

	foodSources.insert_or_assign({ pos.x, pos.y }, new Agent("./Assets/burger.png", pos, map));

}

void Anthill::DeleteFood(const sf::Vector2i& pos)
{
	std::cout << "Deleting Food at " << pos.x << " " << pos.y << std::endl;

	delete foodSources.at({ pos.x, pos.y });
	foodSources.erase({ pos.x, pos.y });

}
