#include "EmptyThreatStencil.h"

EmptyThreatStencil::EmptyThreatStencil()
{
	stencil.push_back(std::pair<CubeCoords, int>(CubeCoords(0, 0, 0), 1));
}
