/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * snake.h                                                       *
 * The Snake class contains player parameters and methods for    *
 * interacting with the field. The main parameters of the snake   *
 * are the coordinates of the body and the direction of movement.*
 *                                                               *
 * Created by DSuhoi (2020) (C++11)                              *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <ncurses.h>
#include "../term/coords.h"
#include "../term/display.h"


const int START_SEG = 3;	// Start number of segments
const int SEG_PLUS = 1;		// Increasing body elements

// Class of player
class Snake {
private:
	Coords *BodyCoords;	// Coordinates of the snake's body
	int snakeLen;		// Number of segments
	int headVector;		// Direction of movement of the snake
public:
	Snake();
	~Snake();
	void InitSnake(Coords SpawnCoords, long MaxSnakeLen); // Setting the initial coordinates
	void EraseSnake();					// Delete parameters of the snake
	void Move(const int vector);		// Function of snake movement
	Coords *GetBodyCoords();			// Pointer to the coordinates of the snake
	int GetVector();		// Get the direction of movement
	int GetSnakeLen();		// Getting the length of the snake
	void IncSnakeLen();		// Increase the length of the snake by a certain number of elements
	Coords InfoHead();		// Information about the coordinates of the head
	void SetHeadCoords(int x, int y);	// Setting the coordinates of the snake's head
};

#endif
