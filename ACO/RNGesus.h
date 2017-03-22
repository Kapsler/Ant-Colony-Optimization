#pragma once

class RNGesus
{
public:
	RNGesus(const unsigned long& ix, const unsigned long& iy, const unsigned long& iz);

	unsigned long GetNumber();

private:
	unsigned long x, y, z;
	unsigned long t = 0;
};