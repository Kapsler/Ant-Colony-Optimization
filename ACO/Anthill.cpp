#include "Anthill.h"
#include <iostream>
#include <chrono>

Anthill::Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr) : Agent(filename, startingIndex, mapPtr)
{
	debugFont.loadFromFile("./Assets/arial.ttf");
	debugText.setFont(debugFont);
	debugText.setCharacterSize(18);
	debugText.setFillColor(sf::Color::White);
	debugText.setOutlineThickness(2);
	debugText.setOutlineColor(sf::Color::Black);

	for(unsigned int i = 0; i < 16; ++i)
	{
		geez.push_back(RNGesus(i, std::chrono::high_resolution_clock::now().time_since_epoch().count(), std::chrono::high_resolution_clock::now().time_since_epoch().count() << i));
	}
}

Anthill::~Anthill()
{
}

void Anthill::FindFood()
{
	float optimalPath = OptimalPathHeuristic();
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

	#pragma omp parallel for
	for(int i = 0; i < numberOfAnts; ++i)
	{
		std::unordered_map<HexData*, HexData*> cameFrom;
		std::vector<HexData*> neighbors;
		sf::Vector2i antPos(positionIndex);
		bool foundFood = false;
		float allTerrains = 0.0f;

		//startpoint
		cameFrom.insert_or_assign(nullptr, map->GetHexDatByIndex(antPos.x, antPos.y));

		//Traverse - Begin

		std::vector<std::pair<float, HexData*>> possibleFields;
		possibleFields.reserve(6);

		while(!foundFood)
		{
			neighbors = map->GetNeighbors(map->GetHexDatByIndex(antPos.x, antPos.y), *map->GetMapPtr());
			possibleFields.clear();
			HexData* nextField = GetNextField(neighbors, cameFrom, possibleFields);
			if(nextField == nullptr)
			{
				//No more options
				break;
			}

			cameFrom.insert_or_assign(map->GetHexDatByIndex(antPos.x, antPos.y), nextField);
			allTerrains += nextField->terrain;
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
			//Using average terrain cost in heurstic
			float pherValue = (optimalPath * (1.0f / (fastPow(allTerrains / static_cast<float>(cameFrom.size()), 2))) / static_cast<float>(cameFrom.size()));
			//float pherValue = (optimalPath / static_cast<float>(cameFrom.size()));

			for(auto it = cameFrom.begin(); it != cameFrom.end(); ++it)
			{
				if(it->first != nullptr)
				{
					#pragma omp critical
					{
						doubleBuffer[it->second->index.x][it->second->index.y]->pheromones += pherValue;
					}
				}
			}
		}

		//Mark - End
	}

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
			if (h->pheromones > pheromoneEpsilon)
			{
				h->pheromones = (1 - rho) * h->pheromones;
			} else
			{
				h->pheromones = 0.0f;
			}
		}
	}

	//Evaporation - End

	FindPheromonePaths(Astarpaths);

}

HexData* Anthill::GetNextField(std::vector<HexData*>& neighbors, const std::unordered_map<HexData*, HexData*>& visited, std::vector<std::pair<float, HexData*>>& possibleFields)
{
	HexData* nextField = nullptr;

	//Find scaling factor
	float scalingFactor = 0;

	auto it = neighbors.begin();
	while(it != neighbors.end())
	{
		if (visited.find(*it) == visited.end() && (*it)->terrain < map->unpassable)
		{
				float pher = (*it)->pheromones;

				if (pher == 0)
				{
					pher = pheromoneEpsilon;
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
			pher = pheromoneEpsilon;
		}

		p = (fastPow(pher, a) * fastPow((1.0f / n->terrain), b)) / (scalingFactor);

		possibleFields.push_back({ p, n });
		
	}

	//Choose random field by possibility
	float randVal = geez[omp_get_thread_num()].GetNumber() / (ULONG_MAX + 1.0f);
	
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
	if(key == sf::Keyboard::Key::O)
	{
		renderPheromoneFlag = !renderPheromoneFlag;
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
	//Pheremone Color
	Hexagon renderHex;

	//Get Max Pheromone Value
	float maxPheromone = pheromoneEpsilon;
	for (const auto& line : *map->GetMapPtr())
	{
		for (const auto& h : line)
		{
			if (h->pheromones > maxPheromone)
			{
				maxPheromone = h->pheromones;
			}
		}
	}

	//Render A* Path
	for (const auto h : Astarpaths)
	{
		renderHex = *h->hex;
		renderHex.setOutlineColor(sf::Color::Red);
		window->draw(renderHex);
	}

	//Render Pheromones
	if(renderPheromoneFlag)
	{
		for (const auto& line : *map->GetMapPtr())
		{
			for (const auto& h : line)
			{
				if (h->pheromones > 0.0f)
				{
					renderHex = *h->hex;
					renderHex.setOutlineColor(sf::Color::Transparent);
					float alpha = std::min(255.0f * (h->pheromones / maxPheromone), 255.0f);
					renderHex.setFillColor(sf::Color(127, 0, 255, alpha));
					window->draw(renderHex);
				}
			}
		}
	}

	Agent::Render(window);

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

int Anthill::OptimalPathHeuristic()
{
	int shortestPath = INT_MAX;

	for(auto it = foodSources.begin(); it != foodSources.end(); ++it)
	{
		int pathlength = GetManhattenDistance(positionIndex, it->second->GetPositionIndex());
		//int pathlength = map->AStarPath(map->GetHexDatByIndex(positionIndex.x, positionIndex.y), map->GetHexDatByIndex(it->second->GetPositionIndex().x, it->second->GetPositionIndex().y), *map->GetMapPtr(), nullptr).size();

		if(pathlength < shortestPath)
		{
			shortestPath = pathlength;
		}
	}

	return shortestPath;
}

void Anthill::FindPheromonePaths(std::vector<HexData*>& out)
{
	out.clear();
	std::vector<HexData*> newPath;
	for (auto it = foodSources.begin(); it != foodSources.end(); ++it)
	{
		newPath = map->AStarPathPheromones(map->GetHexDatByIndex(positionIndex.x, positionIndex.y), map->GetHexDatByIndex(it->second->GetPositionIndex().x, it->second->GetPositionIndex().y), *map->GetMapPtr(), nullptr);

		out.insert(out.end(), newPath.begin(), newPath.end());
	}
}