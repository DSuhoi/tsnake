/*** * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * map.h                                                 *
 * The Map class contains all information about the game *
 * map (size, number of fruits, and borders). This class *
 * implements checking of coordinates and the display of *
 * objects on it.                                        *
 *                                                       *
 * Created by DSuhoi (2020) (C++17)                      *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __MAP_HPP__
#define __MAP_HPP__

#include <list>
#include <ctime>
#include "display.hpp"
#include "coords.hpp"
#include "term_windows.hpp"

// Map and fruit characters
chtype const BORDERCHR = '#';
chtype const FRUITCHR = '$' | COLOR_PAIR(YELLOW);
chtype const EMPTYCHR = ' ';
// Snake characters
chtype const BODYCHR = 'o' | COLOR_PAIR(GREEN);
chtype const HEAD = '@' | COLOR_PAIR(GREEN);
chtype const KILL = 'X' | COLOR_PAIR(RED);

// Width and height of different maps
int const SMALL_WIDTH = 37;
int const SMALL_HEIGHT = 9;

int const MEDIUM_WIDTH = 55;
int const MEDIUM_HEIGHT = 14;

int const BIG_WIDTH = 77;
int const BIG_HEIGHT = 20;

// Width and height of the map window
int const WIDTH = 80;
int const HEIGHT = 22;

auto const SCREEN_WIDTH = [] {
    int width, height;
    getmaxyx(stdscr, height, width);
    return width;
} ();

auto const SCREEN_HEIGHT = [] {
    int width, height;
    getmaxyx(stdscr, height, width);
    return height;
} ();


// Class of map
class Map : public Term_window
{
public:
    Map() = default;      // Constructor
    Map(int select_map_size, int number_fruits);
    ~Map();     // Destructor

    void update(std::list<Coords> &snake);  // Update the images of all objects on the map

    void select_size_map(int select);               // Selecting the map size
    void init_border_coords(Coords snake_coords);   // Setting the borders
    void init_fruit_coords(int number);             // Setting the number of fruits
    void set_fruit_on_map(Coords fruit_coords, std::list<Coords> &snake_coords);      // Creating fruits on the map

    void print_sub_menu_static(const long last_score, const int level);               // Print the static part of the submenu
    void print_sub_menu_active(const long score, time_t &first_time);                 // Print the dynamic part of the submenu

    Coords get_spawn_snake();         // Get the coordinates of the snake spawn
    bool is_snake_tail(Coords coords, std::list<Coords> &snake_coords);               // Checking the player's coordinates
    bool is_fruit(Coords coords);     // Check the coordinates of the fruits
    bool is_border(Coords coords);    // Check the coordinates of the borders
private:
    Term_zone game_map;
    Term_zone status_zone;
    std::list<Coords> borders;        // Pointer to the coordinates of the borders
    std::list<Coords> fruits;         // Pointer to the coordinates of the fruits
    Coords spawn_snake;               // Coordinates of the snake spawn
    
    void border_map();        // Printing borders
};

#endif
