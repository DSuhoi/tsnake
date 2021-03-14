#ifndef __FRUITS_HPP__
#define __FRUITS_HPP__

#include <iostream>
#include "coords.hpp"

class Fruits
{
public:
    Fruits() = default;
    Fruits(int height, int width, int number_fruits_on_map, bool (*is_border)(Coords);

    bool try_set_fruit(Coords coord, int height, int width, bool (*is_border)(Coords));
    bool is_fruit(Coords coord);
private:
    std::list<Coords> _fruits_coords;
};

#endif
