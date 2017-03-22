#pragma once
#include <utility>

struct SimpleHash
{
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2> &p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);

		return h1 ^ ((h2 << 1));
	}
};

inline double fastPow(double a, double b) {
	union {
		double d;
		int x[2];
	} u = { a };
	u.x[1] = static_cast<int>(b * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;
	return u.d;
}

inline int GetManhattenDistance(const sf::Vector2i& a, const sf::Vector2i& b)
{
	int dx, dy;

	dx = b.x - a.x;
	dy = b.y - a.y;

	return abs(dx) + abs(dy);
}