/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * snake.h                                                       *
 * The Snake class contains player parameters and methods for    *
 * interacting with the field. The main parameters of the snake   *
 * are the coordinates of the body and the direction of movement.*
 *                                                               *
 * Created by DSuhoi (2020) (C++11)                              *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __SNAKE_HPP__
#define __SNAKE_HPP__

#include "coords.hpp"
#include "display.hpp"


const int START_SEG = 3;    // Start number of segments
const int SEG_PLUS = 1;     // Increasing body elements

// Class of player
class Snake
{
public:
    Snake();
    ~Snake();
    void init_snake(Coords spawn_coords, long max_snake_len);  // Setting the initial coordinates
    void erase_snake();                                        // Delete parameters of the snake
    void move(const int vector);                               // Function of snake movement
    Coords *get_body_coords();                                 // Pointer to the coordinates of the snake
    int get_vector();                                          // Get the direction of movement
    int get_snake_len();                                       // Getting the length of the snake
    void inc_snake_len();                                      // Increase the length of the snake by a certain number of elements
    Coords info_head();                                        // Information about the coordinates of the head
    void set_head_coords(int x, int y);                        // Setting the coordinates of the snake's head
private:
    Coords *body_coords;                                       // Coordinates of the snake's body
    int snake_len;                                             // Number of segments
    int head_vector;                                           // Direction of movement of the snake
};

#endif
