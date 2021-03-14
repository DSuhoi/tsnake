#include "fruits.hpp"


Fruits::Fruits(int height, int width, int number_fruits_on_map, bool (*is_border)(Coords))
{
    // Checking number of fruits
    if (number_fruits_on_map < 1 ||  99 < number_fruits_on_map)
        number_fruits_on_map = 1;
    
    for (auto i = 0; i < number_fruits_on_map; ++i) {
        Coords random_coords;    // Coordinates of the fruit
        do {     
            random_coords.x = 1 + std::rand() % (width - 1);    // Random coordinates
            random_coords.y = 1 + std::rand() % (height - 1);
        } while (is_fruit(random_coords) || is_border(random_coords));
        fruits.push_back(random_coords);   // Checking and assigning coordinates
    }
}

bool Fruits::try_set_fruit(Coords coord, int height, int width, bool (*is_border)(Coords))
{
    int error_counter = 0;   // Repeat count
    
    for (auto &&fruit : _fruits_coords) {
        Coords random_coords;
        if (fruit_coords == fruit) {
            do { 
                // If more than 1000 repetitions have passed, we exit the loop ()
                if (error_counter++ > 1000) 
                    return false; 
                random_coords.x = 1 + std::rand() % (width - 1);    // Random coordinates
                random_coords.y = 1 + std::rand() % (height - 1);
                // Check the coordinates
            } while (is_fruit(random_coords) || is_border(random_coords));
            fruit = random_coords; // Assigning the correct coordinates
        } 
    }
    return true;
}

bool Fruits::is_fruit(Coords coord)
{
    for (auto const &fruit : _fruits_coords)
        if (coords == fruit)
            return true;
    return false;
}
