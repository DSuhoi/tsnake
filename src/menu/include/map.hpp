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
#include <optional>
#include "display.hpp"
#include "coords.hpp"
#include "term_windows.hpp"

// Map and fruit characters
const chtype BORDERCHR = '#';
const chtype FRUITCHR = '$' | COLOR_PAIR(YELLOW);
const chtype EMPTYCHR = ' ';
// Snake characters
const chtype BODYCHR = 'o' | COLOR_PAIR(GREEN);
const chtype HEAD = '@' | COLOR_PAIR(GREEN);
const chtype KILL = 'X' | COLOR_PAIR(RED);

// Width and height of different maps
const int SMALL_WIDTH = 37;
const int SMALL_HEIGHT = 9;

const int MEDIUM_WIDTH = 55;
const int MEDIUM_HEIGHT = 14;

const int BIG_WIDTH = 77;
const int BIG_HEIGHT = 20;

// Width and height of the map window
const int WIDTH = 80;
const int HEIGHT = 22;

const auto SCREEN_WIDTH = [] {
    int width, height;
    getmaxyx(stdscr, height, width);
    return width;
} ();

const auto SCREEN_HEIGHT = [] {
    int width, height;
    getmaxyx(stdscr, height, width);
    return height;
} ();

class Map_zone : public Term_zone
{
public:
    Map_zone(int height, int width, int pos_y, int pos_x);
    ~Map_zone() = default;

    void init_border_coords(Coords snake_coords);   // Setting the borders
    void init_fruit_coords(int number);             // Setting the number of fruits

    void set_fruit_on_map(Coords fruit_coords, std::list<Coords> &snake_coords);      // Creating fruits on the map
private:
    std::optional<std::list<Coords>> borders;        // Pointer to the coordinates of the borders
    std::list<Coords> fruits;         // Pointer to the coordinates of the fruits

    void border_map();        // Printing borders
};

// Class of map
class Map_window : public Term_window
{
public:
    Map_window() = default;      // Constructor
    Map_window(int select_map_size, int number_fruits);
    ~Map_window();     // Destructor

    void update(std::list<Coords> &snake);  // Update the images of all objects on the map

    void select_size_map(int select);      // Selecting the map size

    void print_sub_menu_static(const long last_score, const int level);               // Print the static part of the submenu
    void print_sub_menu_active(const long score, time_t &first_time);                 // Print the dynamic part of the submenu

    Coords get_spawn_snake();         // Get the coordinates of the snake spawn
    bool is_snake_tail(Coords coords, std::list<Coords> &snake_coords);               // Checking the player's coordinates
    bool is_fruit(Coords coords);     // Check the coordinates of the fruits
    bool is_border(Coords coords);    // Check the coordinates of the borders
private:
    Map_zone game_map;
    Term_zone status_zone;
    Coords spawn_snake;               // Coordinates of the snake spawn
};

#endif
