#include "Agent.h"
#include <iostream>

Agent::Agent(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr)
{
	map = mapPtr;
	positionIndex = startingIndex;

	//Load Sprite
	sf::Texture* tex = new sf::Texture();
	if(!tex->loadFromFile(filename))
	{
		std::cerr << "Error loading texture " << filename << std::endl;
	}
	tex->setSmooth(true);
	sprite.setTexture(*tex);
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setPosition(map->GetPositionByIndex(positionIndex));

	float scalex = map->GetHexSize() / sprite.getGlobalBounds().width ;
	float scaley = map->GetHexSize() / sprite.getGlobalBounds().height;
	float uniformscale = (scalex < scaley ? scalex : scaley) * 1.2f;

	sprite.scale(uniformscale, uniformscale);
}

Agent::~Agent()
{

}

void Agent::Move()
{
	HexData* nextHex = GetNextField();

	RotateStencil(nextHex);
	positionIndex = nextHex->index;
	position = nextHex->hex->getPosition();
	sprite.setPosition(position);
}

void Agent::Render(sf::RenderWindow* window)
{
	window->draw(sprite);
}

void Agent::DebugRender(sf::RenderWindow* window)
{

}

ThreatStencil* Agent::GetThreatStencil() const
{
	return stencil;
}

void Agent::SetThreatStencil(ThreatStencil* ostencil)
{
	stencil = ostencil;
}

void Agent::RotateStencil(HexData* target)
{
	if(stencil)
	{
		stencil->RotateToTarget(GetCurrentHex(), target);
	}
}

HexData* Agent::GetCurrentHex() const
{
	return map->GetHexDatByIndex(GetPositionIndex().x, GetPositionIndex().y);
}

sf::Vector2i Agent::GetPositionIndex() const
{
	return positionIndex;
}

HexData* Agent::GetNextField()
{
	bool result = false;

	pathToFollow.clear();

	if (wayPoints.size() > 0)
	{
		HexData* target = wayPoints[targetindex%wayPoints.size()];

		pathToFollow = map->AStarPath((*map->GetMapPtr())[positionIndex.x][positionIndex.y], target, *map->GetMapPtr(), this);

		if (pathToFollow.size() > 1)
		{
			HexData* nextField = pathToFollow[1];

			if (nextField->index == target->index)
			{
				targetindex++;
			}

			return nextField;
		}
	} 

	return map->GetHexDatByIndex(positionIndex.x, positionIndex.y);
}

void Agent::SetWayPoints(std::vector<HexData*> otherWayPoints)
{
	wayPoints = otherWayPoints;
}

