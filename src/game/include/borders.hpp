#ifndef __BORDERS_HPP__
#define __BORDERS_HPP__

#include <iostream>
#include "coords.hpp"

class Borders
{
public:
    Borders() = default;
    Borders(int height, int width);

    void init_extra_borders(int number_borders_inside);
    bool is_border(Coords coord);
private:
    std::list<Coords> _borders_coords;
};

#endif
