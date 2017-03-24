#include "RNGesus.h"

RNGesus::RNGesus(const unsigned long& xseed, const unsigned long& yseed, const unsigned long& zseed)
{
	x = xseed;
	y = yseed;
	z = zseed;
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
