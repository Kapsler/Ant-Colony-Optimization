#include "Hexagon.h"
#include <SFML/Graphics.hpp>
#include "Moving.h"
#include "Interactive.h"
#include "Renderable.h"
#include "Map.h"
#include "Agent.h"
#include <iostream>
#include <chrono>
#include "Anthill.h"

const bool vsync = false;
const float screenWidth = 2000;
const float screenHeight = 1400;

sf::RenderWindow *window;
std::vector<Renderable*> toRender;
std::vector<Interactive*> toInteract;
std::vector<Moving*> toMove;

void GenerateWorld()
{
	Map* map = new Map(screenWidth, screenHeight, "./Assets/terrain50x30.png");
	toRender.push_back(map);
	toInteract.push_back(map);

	Anthill* anthill = new Anthill("./Assets/anthill.png", sf::Vector2i(20, 12), map);
	toRender.push_back(anthill);
	toInteract.push_back(anthill);
	toMove.push_back(anthill);

}

int main()
{
	//Initialize Random Generator and clock
	srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	sf::Clock deltaClock;
	float fpsDelay = 0;
	float moveDelay = 0;

	//Open Window
	window = new sf::RenderWindow(sf::VideoMode(static_cast<unsigned int>(screenWidth), static_cast<unsigned int>(screenHeight)), "ACO");
	window->setVerticalSyncEnabled(vsync);

	//Generate World
	GenerateWorld();

	while (window->isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();

		//Update FPS
		fpsDelay += deltaTime;
		if (fpsDelay > 1.0f)
		{
			float fps = 1.0f / (deltaTime);
			window->setTitle("ACO (" + std::to_string(static_cast<int>(fps)) + ")");
			fpsDelay = 0.0f;
		}

		sf::Event event;

		//Interacting
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}

			if(event.type == sf::Event::KeyPressed)
			{
				if(event.key.code == sf::Keyboard::Escape)
				{
					window->close();
				}

				for (const auto& i : toInteract)
				{
					i->HandleKeyboard(event.key.code);
				}
			}

			if(event.type == sf::Event::MouseMoved)
			{
				sf::Vector2f mpos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

				for (const auto& i : toInteract)
				{
					i->HandleMouse(mpos);
				}
			}
			
			if(event.type == sf::Event::MouseButtonPressed)
			{
				for (const auto& i : toInteract)
				{
					i->HandleMouse(event.mouseButton.button);
				}
			}

		}

	

		//Movement
		moveDelay += deltaTime;
		if (moveDelay > 0.0001f)
		{
			for (auto& m : toMove)
			{
				m->Move();
			}
			moveDelay = 0.0f;
		}

		//Rendering

		window->clear();

		for(const auto& r : toRender)
		{
			r->Render(window);
			r->DebugRender(window);
		}

		window->display();

	}

	return 0;
}
