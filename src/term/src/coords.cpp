#include "coords.hpp"

// Overloading comparison operators for coordinates
bool operator==(const Coords &cd1, const Coords &cd2)
{
    return cd1.x == cd2.x && cd1.y == cd2.y;
}

bool operator!=(const Coords &cd1, const Coords &cd2)
{
    return !(cd1 == cd2);
}
