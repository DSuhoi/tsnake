/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * snake.h                                                       *
 * The Snake class contains player parameters and methods for    *
 * interacting with the field. The main parameters of the snake   *
 * are the coordinates of the body and the direction of movement.*
 *                                                               *
 * Created by DSuhoi (2020) (C++17)                              *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

#include <list>
#include "coords.hpp"
#include "display.hpp"


int const START_SEG = 3;    // Start number of segments
int const SEG_PLUS = 1;     // Increasing body elements

// Class of player
class Snake
{
public:
    Snake() = default;
    Snake(Coords spawn_coords);
    ~Snake();
    void move(int const vector);          // Function of snake movement
    std::list<Coords> &get_body_coords(); // Pointer to the coordinates of the snake
    int get_vector();                     // Get the direction of movement
    int get_snake_len();                  // Getting the length of the snake
    void inc_snake_len();                 // Increase the length of the snake by a certain number of elements
    Coords info_head();                   // Information about the coordinates of the head
    void set_head_coords(int x, int y);   // Setting the coordinates of the snake's head
private:
    std::list<Coords> body_coords;        // Coordinates of the snake's body
    int head_vector;                      // Direction of movement of the snake
};

#endif
