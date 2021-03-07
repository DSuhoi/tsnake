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
#include "coords.hpp"
#include "display.hpp"

// Map and fruit characters
const char BORDERCHR = '#';
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

// Class of map
class Map
{
private:
    static int height;               // Width of the map
    static int width;                // Height of the map
    static WINDOW *map;              // Map window
    static Coords *borders;          // Pointer to the coordinates of the borders
    static Coords *fruits;           // Pointer to the coordinates of the fruits
    static Coords spawn_snake;       // Coordinates of the snake spawn
    static unsigned int num_fruits;  // number of fruits
    static unsigned int num_border;  // number of borders
    
    static void border_map();        // Printing borders
public:
    Map() = delete;                                        // Constructor
    static void init_map();                                // Configure the map
    static void erase_map();                               // Erase the map parametrs
    static void select_size_map(int select);               // Selecting the map size
    static void init_border_coords(Coords snake_coords);   // Setting the borders
    static void init_fruit_coords(int number);             // Setting the number of fruits
    static void set_fruit_on_map(Coords fruit_coords, std::list<Coords> &snake_coords, int snake_len);      // Creating fruits on the map
    static void update_map(std::list<Coords> &snake, int snake_len);  // Update the images of all objects on the map

    static void set_map(int x, int y, chtype color);       // Set character on the map
    static void print_sub_menu_static(const long last_score, const int level);                   // Print the static part of the submenu
    static void print_sub_menu_active(const long score, time_t &first_time);                     // Print the dynamic part of the submenu
    static void border_coords_cpy(Coords *border_coords, int num_coords, Coords spawn_coords);   // Copy the coordinates
    static int get_height();                 // Return the map height
    static int get_width();                  // Return the map width

    static Coords get_spawn_snake();         // Get the coordinates of the snake spawn
    static bool is_snake(Coords coords, std::list<Coords> &snake_coords, int snake_len);                    // Checking the player's coordinates
    static bool is_fruit(Coords coords);     // Check the coordinates of the fruits
    static bool is_border(Coords coords);    // Check the coordinates of the borders
};

#endif
