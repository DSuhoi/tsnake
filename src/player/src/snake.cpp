#include "snake.hpp"
#include <iterator>

// Constructor
Snake::Snake(Coords spawn_coords)
{ 
    // Create the snake (body coordinates)
    // Setting the coordinates of the appearance of the snake
    body_coords.push_back(spawn_coords);
    head_vector = KEY_RIGHT;
    for (auto i = 0; i <= START_SEG; ++i)
        body_coords.push_back(spawn_coords);
}   

// Destructor
Snake::~Snake()
{ 
    // Delete the coordinates of the snake's body
    body_coords.clear();
}   

// Function of snake movement
void Snake::move(int const vector)
{
    // Assign new characters further (to save the direction of travel) 
    for (auto it = body_coords.rbegin(); it != (--body_coords.rend());) {
        auto current_it = it++;
        *current_it = *it;
    }
    
    // Determining the direction and moving there
    switch (head_vector) {
    case KEY_LEFT:
        if (vector != KEY_RIGHT)
            head_vector = vector;
        --body_coords.front().x;
        break;
    case KEY_RIGHT:
        if (vector != KEY_LEFT)
            head_vector = vector;
        ++body_coords.front().x; 
        break;
    case KEY_UP:
        if (vector != KEY_DOWN)
            head_vector = vector;
        --body_coords.front().y; 
        break;
    case KEY_DOWN:
        if (vector != KEY_UP)
            head_vector = vector;
        ++body_coords.front().y; 
        break;
    default: 
        break;
    }
}

// Increase the length of the snake by a certain number of elements
void Snake::inc_snake_len()
{
    auto const last_pos_body = body_coords.back();
    for (auto i = 0; i < SEG_PLUS; ++i)
        body_coords.push_back(last_pos_body);
}

// Information about the coordinates of the head
Coords Snake::info_head()
{ 
    return body_coords.front(); 
}   

// Setting the coordinates of the snake's head
void Snake::set_head_coords(int x, int y)
{ 
    body_coords.front().x = x; 
    body_coords.front().y = y; 
}

// Getting the length of the snake
int Snake::get_snake_len()
{ 
    return body_coords.size() - 1;
}

// Pointer to the coordinates of the snake
std::list<Coords> &Snake::get_body_coords()
{ 
    return body_coords; 
}

// Get the direction of movement
int Snake::get_vector()
{ 
    return head_vector; 
}
