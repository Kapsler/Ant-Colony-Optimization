#include "RNGesus.h"

RNGesus::RNGesus(const unsigned long& ix, const unsigned long& iy, const unsigned long& iz)
{
	x = ix;
	y = iy;
	z = iz;
}

unsigned long RNGesus::GetNumber()
{
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}
