#pragma once

class RNGesus
{
public:
	RNGesus(const unsigned long& xseed, const unsigned long& yseed, const unsigned long& zseed);

	unsigned long GetNumber();

private:
	unsigned long x, y, z;
	unsigned long t = 0;
};