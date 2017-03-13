#pragma once
#include "Agent.h"
#include <unordered_map>

class Anthill : public Agent, public Interactive
{
public:
	struct SimpleHash
	{
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2> &p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			return h1 ^ h2;
		}
	};

	const int numberOfAnts = 1000;

	Anthill(const std::string filename, sf::Vector2i startingIndex, Map* mapPtr);
	~Anthill();

	void FindFood();
	HexData* GetNextField(const std::vector<HexData*>& neighbors, const std::vector<HexData*>& visited);

	virtual void HandleKeyboard(sf::Keyboard::Key key);
	virtual void HandleMouse(sf::Vector2f& mousePosition);
	virtual void HandleMouse(sf::Mouse::Button mb);
	virtual void Render(sf::RenderWindow* window);
	virtual void Move() override;
	void DebugRender(sf::RenderWindow* window) override;

	void SpawnFood(const sf::Vector2i& pos);
	void DeleteFood(const sf::Vector2i& pos);

	int FindOptimalPath();

	std::unordered_map<std::pair<int, int>, Agent*, SimpleHash> foodSources;
	std::vector<HexData*> debugDraw;

	sf::Text debugText;
	sf::Font debugFont;
};
