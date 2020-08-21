#include "snake.h"

// Constructor
Snake::Snake()
{ 
	BodyCoords = nullptr;
}	

// Destructor
Snake::~Snake()
{ 
	EraseSnake();
}	

// Setting the initial coordinates
void Snake::InitSnake(Coords SpawnCoords, long MaxSnakeLen)
{
	// Create the snake (body coordinates)
	BodyCoords = new Coords[MaxSnakeLen];
	snakeLen = START_SEG;
	// Setting the coordinates of the appearance of the snake
	BodyCoords[0] = SpawnCoords;
	headVector = KEY_RIGHT;
	for(int i = snakeLen; i >= 0; i--) BodyCoords[i] = BodyCoords[0];
}

// Delete parameters of the snake
void Snake::EraseSnake()
{
	// Delete the coordinates of the snake's body
	if(BodyCoords!=nullptr){ 
		delete [] BodyCoords; 
		BodyCoords = nullptr;
	}	
	
	snakeLen = START_SEG;
	headVector = 0;
}

// Function of snake movement
void Snake::Move(const int vector)
{
	// Assign new characters further (to save the direction of travel)
	for(int i = snakeLen; i > 0; i--){
		BodyCoords[i] = BodyCoords[i-1];
	}
	
	// Determining the direction and moving there
	switch(headVector){
	case KEY_LEFT:
		if(vector != KEY_RIGHT){ 
			headVector = vector;
		}
			BodyCoords[0].x--;
			break;
	case KEY_RIGHT:
		if(vector != KEY_LEFT){ 
			headVector = vector;
		} 
			BodyCoords[0].x++; 
			break;
	case KEY_UP:
		if(vector != KEY_DOWN){ 
			headVector = vector;
		} 
			BodyCoords[0].y--; 
			break;
	case KEY_DOWN:
		if(vector != KEY_UP){ 
			headVector = vector;
		} 
			BodyCoords[0].y++; 
			break;
	default: 
			break;
	}
}

// Increase the length of the snake by a certain number of elements
void Snake::IncSnakeLen()
{
	snakeLen+=SEG_PLUS; // Increase the length if you eat a fruit
	for(int seg = snakeLen; seg > snakeLen-SEG_PLUS; seg--)
		BodyCoords[seg] = BodyCoords[snakeLen - SEG_PLUS];
	 
}

// Information about the coordinates of the head
Coords Snake::InfoHead()
{ 
	return BodyCoords[0]; 
}	

// Setting the coordinates of the snake's head
void Snake::SetHeadCoords(int x, int y)
{ 
	BodyCoords[0].x = x; 
	BodyCoords[0].y = y; 
}

// Getting the length of the snake
int Snake::GetSnakeLen()
{ 
	return snakeLen; 
}

// Pointer to the coordinates of the snake
Coords *Snake::GetBodyCoords()
{ 
	return BodyCoords; 
}

// Get the direction of movement
int Snake::GetVector()
{ 
	return headVector; 
}
