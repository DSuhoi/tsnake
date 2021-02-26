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

#include <ncurses.h>
#include "coords.hpp"
#include "display.hpp"


const int START_SEG = 3;    // Start number of segments
const int SEG_PLUS = 1;     // Increasing body elements

// Class of player
class Snake {
private:
    Coords *bodyCoords;     // Coordinates of the snake's body
    int snakeLen;           // Number of segments
    int headVector;         // Direction of movement of the snake
public:
    Snake();
    ~Snake();
    void initSnake(Coords SpawnCoords, long MaxSnakeLen); // Setting the initial coordinates
    void eraseSnake();                  // Delete parameters of the snake
    void move(const int vector);        // Function of snake movement
    Coords *getBodyCoords();            // Pointer to the coordinates of the snake
    int getVector();                    // Get the direction of movement
    int getSnakeLen();                  // Getting the length of the snake
    void incSnakeLen();                 // Increase the length of the snake by a certain number of elements
    Coords infoHead();                  // Information about the coordinates of the head
    void setHeadCoords(int x, int y);   // Setting the coordinates of the snake's head
};

#endif
