#include "coords.h"

// Overloading comparison operators for coordinates
bool operator==(Coords &cd1, Coords &cd2)
{
	return (cd1.x == cd2.x && cd1.y == cd2.y) ? true : false;
}

bool operator!=(Coords &cd1, Coords &cd2)
{
	return !(cd1 == cd2);
}
