#include "snake.hpp"

// Constructor
Snake::Snake()
{ 
    body_coords = nullptr;
}   

// Destructor
Snake::~Snake()
{ 
    erase_snake();
}   

// Setting the initial coordinates
void Snake::init_snake(Coords spawn_coords, long max_snake_len)
{
    // Create the snake (body coordinates)
    body_coords = new Coords[max_snake_len];
    snake_len = START_SEG;
    // Setting the coordinates of the appearance of the snake
    body_coords[0] = spawn_coords;
    head_vector = KEY_RIGHT;
    for (int i = snake_len; i >= 0; --i)
        body_coords[i] = body_coords[0];
}

// Delete parameters of the snake
void Snake::erase_snake()
{
    // Delete the coordinates of the snake's body
    if (body_coords != nullptr) { 
        delete [] body_coords; 
        body_coords = nullptr;
    }   
    
    snake_len = START_SEG;
    head_vector = 0;
}

// Function of snake movement
void Snake::move(const int vector)
{
    // Assign new characters further (to save the direction of travel)
    for (int i = snake_len; i > 0; --i)
        body_coords[i] = body_coords[i - 1];
    
    // Determining the direction and moving there
    switch (head_vector) {
    case KEY_LEFT:
        if (vector != KEY_RIGHT)
            head_vector = vector;
        --body_coords[0].x;
        break;
    case KEY_RIGHT:
        if (vector != KEY_LEFT)
            head_vector = vector;
        ++body_coords[0].x; 
        break;
    case KEY_UP:
        if (vector != KEY_DOWN)
            head_vector = vector;
        --body_coords[0].y; 
        break;
    case KEY_DOWN:
        if (vector != KEY_UP)
            head_vector = vector;
        ++body_coords[0].y; 
        break;
    default: 
        break;
    }
}

// Increase the length of the snake by a certain number of elements
void Snake::inc_snake_len()
{
    snake_len += SEG_PLUS; // Increase the length if you eat a fruit
    for (int seg = snake_len; seg > snake_len - SEG_PLUS; seg--)
        body_coords[seg] = body_coords[snake_len - SEG_PLUS];
     
}

// Information about the coordinates of the head
Coords Snake::info_head()
{ 
    return body_coords[0]; 
}   

// Setting the coordinates of the snake's head
void Snake::set_head_coords(int x, int y)
{ 
    body_coords[0].x = x; 
    body_coords[0].y = y; 
}

// Getting the length of the snake
int Snake::get_snake_len()
{ 
    return snake_len; 
}

// Pointer to the coordinates of the snake
Coords *Snake::get_body_coords()
{ 
    return body_coords; 
}

// Get the direction of movement
int Snake::get_vector()
{ 
    return head_vector; 
}
