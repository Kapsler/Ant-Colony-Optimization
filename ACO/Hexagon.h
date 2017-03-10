#pragma once
#include <SFML/Graphics.hpp>

class Hexagon : public sf::Shape
{
public:
	Hexagon();
	Hexagon(float radius);

	inline std::size_t  getPointCount(void)  const override;
	sf::Vector2f getPoint(std::size_t index) const override;

	float width(void) const { return m_width; };
	float height(void) const { return m_height; };

	~Hexagon() override;

private:

	std::vector<sf::Vector2f> m_cornerOffsets;
	float m_radius;
	float m_width, m_height;
};