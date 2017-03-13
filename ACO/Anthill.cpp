#include "Anthill.h"
#include <iostream>

Anthill::Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr) : Agent(filename, startingIndex, mapPtr)
{
	debugFont.loadFromFile("./Assets/arial.ttf");
	debugText.setFont(debugFont);
	debugText.setCharacterSize(18);
	debugText.setFillColor(sf::Color::White);
	debugText.setOutlineThickness(2);
	debugText.setOutlineColor(sf::Color::Black);
}

Anthill::~Anthill()
{
}

void Anthill::FindFood()
{
	debugDraw.clear();

	float optimalPath = FindOptimalPath();
	float rho = 0.2f;
	std::vector<std::vector<HexData*>> doubleBuffer;
	std::vector<std::vector<HexData*>> mapptr = *(map->GetMapPtr());

	doubleBuffer.resize(mapptr.size());

	for(int i = 0; i < doubleBuffer.size(); ++i)
	{
		doubleBuffer[i].resize(mapptr[i].size());
	}

	for(int i = 0; i < doubleBuffer.size(); ++i)
	{
		for (int j = 0; j < doubleBuffer[i].size(); ++j)
		{
			doubleBuffer[i][j] = new HexData();
			*(doubleBuffer[i][j]) = *(mapptr[i][j]);
		}
	}

	#pragma omp parallel 
	{
	
		#pragma omp for
		for(int i = 0; i < numberOfAnts; ++i)
		{
			std::unordered_map<HexData*, HexData*> cameFrom;
			std::vector<HexData*> neighbors;
			sf::Vector2i antPos(positionIndex);
			bool foundFood = false;

			//startpoint
			cameFrom.insert_or_assign(nullptr, map->GetHexDatByIndex(antPos.x, antPos.y));

			//Traverse - Begin

			while(!foundFood)
			{
				neighbors = map->GetNeighbors(map->GetHexDatByIndex(antPos.x, antPos.y), *map->GetMapPtr());
				HexData* nextField = GetNextField(neighbors, cameFrom);
				if(nextField == nullptr)
				{
					//No more options
					break;
				}

				cameFrom.insert_or_assign(map->GetHexDatByIndex(antPos.x, antPos.y), nextField);
				antPos = nextField->index;

				if(foodSources.find({antPos.x, antPos.y}) != foodSources.end())
				{
					foundFood = true;
				}
			}

			//Traverse - End

			//Mark - Begin
			if(foundFood)
			{
				for(auto it = cameFrom.begin(); it != cameFrom.end(); ++it)
				{
					#pragma omp critical
					{
						doubleBuffer[it->second->index.x][it->second->index.y]->pheromones += optimalPath / static_cast<float>(cameFrom.size());
					}
				}
			}

			//Mark - End
		}


	}

	mapptr = *map->GetMapPtr();

	for (int i = 0; i < doubleBuffer.size(); ++i)
	{
		for (int j = 0; j < doubleBuffer[i].size(); ++j)
		{
			*(mapptr[i][j]) = *(doubleBuffer[i][j]);
			delete doubleBuffer[i][j];
		}
	}


	//Evaporation - Begin

	for (auto line : *map->GetMapPtr())
	{
		for (auto h : line)
		{
			if (h->pheromones > 0.1f)
			{
				h->pheromones = (1 - rho) * h->pheromones;
			} else
			{
				h->pheromones = 0.0f;
			}
		}
	}


	//Evaporation - End

}

HexData* Anthill::GetNextField(std::vector<HexData*>& neighbors, const std::unordered_map<HexData*, HexData*>& visited)
{
	HexData* nextField = nullptr;
	std::vector<std::pair<float, HexData*>> possibleFields;
	possibleFields.reserve(6);

	//Find scaling factor
	float scalingFactor = 0;
	float a = 1.5f, b = 1.0f;

	auto it = neighbors.begin();
	while(it != neighbors.end())
	{
		if (visited.find(*it) == visited.end() && (*it)->terrain < map->unpassable)
		{
				float pher = (*it)->pheromones;

				if (pher == 0)
				{
					pher = 0.00001f;
				}

				scalingFactor += (fastPow(pher, a) * fastPow((1.0f / (*it)->terrain), b));
			++it;
		} else
		{
			it = neighbors.erase(it);
		}
	}

	//get possibilities per field
	for(const auto& n : neighbors)
	{
		float p = 0;
		float pher = n->pheromones;

		if (pher == 0)
		{
			pher = 0.00001f;
		}

		p = (fastPow(pher, a) * fastPow((1.0f / n->terrain), b)) / (scalingFactor);

		possibleFields.push_back({ p, n });
		
	}

	//Choose random field by possibility
	float randVal = (static_cast<double>(rand()) / (RAND_MAX));


	for(const auto& f : possibleFields)
	{
		if(randVal < f.first)
		{
			nextField = f.second;
			break;
		} 

		randVal -= f.first;
	}

	return nextField;
}

void Anthill::HandleKeyboard(sf::Keyboard::Key key)
{

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

	//Pheremone Color
	Hexagon pheromoneHex;
	for(auto line : *map->GetMapPtr())
	{
		for(auto h : line)
		{
			if(h->pheromones > 0.0f)
			{
				pheromoneHex = *h->hex;
				pheromoneHex.setOutlineColor(sf::Color::Transparent);
				float alpha = std::min(255.0f * (h->pheromones / static_cast<float>(numberOfAnts)), 255.0f);
				pheromoneHex.setFillColor(sf::Color(127, 0, 255, alpha));
				window->draw(pheromoneHex);
			}
		}
	}

	//Render Food
	for(auto it = foodSources.begin(); it != foodSources.end(); ++it)
	{
		it->second->Render(window);
	}
}

void Anthill::Move()
{
	FindFood();
}

void Anthill::DebugRender(sf::RenderWindow* window)
{

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

int Anthill::FindOptimalPath()
{
	int shortestPath = INT_MAX;

	for(auto it = foodSources.begin(); it != foodSources.end(); ++it)
	{
		int pathlength = map->AStarPath(map->GetHexDatByIndex(positionIndex.x, positionIndex.y), map->GetHexDatByIndex(it->first.first, it->first.second), *map->GetMapPtr(), nullptr).size();

		if(pathlength < shortestPath)
		{
			shortestPath = pathlength;
		}
	}

	return shortestPath;
}
