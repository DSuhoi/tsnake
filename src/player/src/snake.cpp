#include "snake.hpp"

// Constructor
Snake::Snake()
{ 
    bodyCoords = nullptr;
}   

// Destructor
Snake::~Snake()
{ 
    eraseSnake();
}   

// Setting the initial coordinates
void Snake::initSnake(Coords SpawnCoords, long MaxSnakeLen)
{
    // Create the snake (body coordinates)
    bodyCoords = new Coords[MaxSnakeLen];
    snakeLen = START_SEG;
    // Setting the coordinates of the appearance of the snake
    bodyCoords[0] = SpawnCoords;
    headVector = KEY_RIGHT;
    for (int i = snakeLen; i >= 0; --i)
        bodyCoords[i] = bodyCoords[0];
}

// Delete parameters of the snake
void Snake::eraseSnake()
{
    // Delete the coordinates of the snake's body
    if (bodyCoords != nullptr) { 
        delete [] bodyCoords; 
        bodyCoords = nullptr;
    }   
    
    snakeLen = START_SEG;
    headVector = 0;
}

// Function of snake movement
void Snake::move(const int vector)
{
    // Assign new characters further (to save the direction of travel)
    for (int i = snakeLen; i > 0; --i)
        bodyCoords[i] = bodyCoords[i - 1];
    
    // Determining the direction and moving there
    switch (headVector) {
    case KEY_LEFT:
        if (vector != KEY_RIGHT)
            headVector = vector;
        --bodyCoords[0].x;
        break;
    case KEY_RIGHT:
        if (vector != KEY_LEFT)
            headVector = vector;
        ++bodyCoords[0].x; 
        break;
    case KEY_UP:
        if (vector != KEY_DOWN)
            headVector = vector;
        --bodyCoords[0].y; 
        break;
    case KEY_DOWN:
        if (vector != KEY_UP)
            headVector = vector;
        ++bodyCoords[0].y; 
        break;
    default: 
        break;
    }
}

// Increase the length of the snake by a certain number of elements
void Snake::incSnakeLen()
{
    snakeLen += SEG_PLUS; // Increase the length if you eat a fruit
    for (int seg = snakeLen; seg > snakeLen - SEG_PLUS; seg--)
        bodyCoords[seg] = bodyCoords[snakeLen - SEG_PLUS];
     
}

// Information about the coordinates of the head
Coords Snake::infoHead()
{ 
    return bodyCoords[0]; 
}   

// Setting the coordinates of the snake's head
void Snake::setHeadCoords(int x, int y)
{ 
    bodyCoords[0].x = x; 
    bodyCoords[0].y = y; 
}

// Getting the length of the snake
int Snake::getSnakeLen()
{ 
    return snakeLen; 
}

// Pointer to the coordinates of the snake
Coords *Snake::getBodyCoords()
{ 
    return bodyCoords; 
}

// Get the direction of movement
int Snake::getVector()
{ 
    return headVector; 
}
