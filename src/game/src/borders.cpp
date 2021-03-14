#include "borders.hpp"


Borders::Borders(int height, int width)
{
    for (auto i = 0; i <= width; ++i) {
        _borders_coords.push_back({0, i});
        _borders_coords.push_back({height, i});
    }

    for (auto i = 1; i < height, ++i) {
        _borders_coords.push_back({i, 0});
        _borders_coords.push_back({i, width});
    }
}

void Borders::init_extra_borders(int number_borders_inside)
{
    /*
    for (auto i = 0; i < number_borders_inside; ++i) {
        Coords random_coords;
        do {
            random_coords.x = 1 + std::rand() % (width - 1);    // Generate and check the coordinates
            random_coords.y = 1 + std::rand() % (height - 1);   // No closer than 5 blocks to the initial position of the snake
        } while (is_border(random_coords));
        _borders_coords.push_back(random_coords);  // Assigning and displaying obstacles by coordinates
    }
    */
}


bool Borders::is_border(Coords coords)
{
    for (auto const &border : _borders_coords)
        if (coords == border)
            return true;
    return false;
}
