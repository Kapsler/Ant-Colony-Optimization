#pragma once

struct CubeCoords
{
	int x;
	int y;
	int z;

	CubeCoords()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	CubeCoords(int ix, int iy, int iz)
	{
		x = ix;
		y = iy;
		z = iz;
	}

	CubeCoords(const CubeCoords& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	CubeCoords operator+ (const CubeCoords& r) const
	{
		CubeCoords ret;

		ret.x = this->x + r.x;
		ret.y = this->y + r.y;
		ret.z = this->z + r.z;

		return ret;
	}

	CubeCoords& operator+= (const CubeCoords& r)
	{
		this->x += r.x;
		this->y += r.y;
		this->z += r.z;
		return *this;
	}

	bool operator!= (const CubeCoords& r) const
	{
		return !(this->x == r.x && this->y == r.y && this->z == r.z);

	}
};