#include "snake.hpp"
#include <iterator>

// Constructor
Snake::Snake(boost::uuids::uuid uid, Coords spawn_coords)
{ 
    _uid = uid;
    // Create the snake (body coordinates)
    // Setting the coordinates of the appearance of the snake
    _body_coords.push_back(spawn_coords);
    _head_vector = KEY_RIGHT;
    for (auto i = 0; i <= START_SEG; ++i)
        _body_coords.push_back(spawn_coords);
}   

// Destructor
Snake::~Snake()
{ 
    // Delete the coordinates of the snake's body
    _body_coords.clear();
}   

// Function of snake movement
void Snake::move(int const vector)
{
    // Assign new characters further (to save the direction of travel) 
    for (auto it = _body_coords.rbegin(); it != (--_body_coords.rend());) {
        auto current_it = it++;
        *current_it = *it;
    }
    
    // Determining the direction and moving there
    switch (_head_vector) {
    case KEY_LEFT:
        if (vector != KEY_RIGHT)
            _head_vector = vector;
        --_body_coords.front().x;
        break;
    case KEY_RIGHT:
        if (vector != KEY_LEFT)
            _head_vector = vector;
        ++_body_coords.front().x; 
        break;
    case KEY_UP:
        if (vector != KEY_DOWN)
            _head_vector = vector;
        --_body_coords.front().y; 
        break;
    case KEY_DOWN:
        if (vector != KEY_UP)
            _head_vector = vector;
        ++_body_coords.front().y; 
        break;
    default: 
        break;
    }
}

// Increase the length of the snake by a certain number of elements
void Snake::inc_snake_len()
{
    auto const last_pos_body = _body_coords.back();
    for (auto i = 0; i < SEG_PLUS; ++i)
        _body_coords.push_back(last_pos_body);
}

// Information about the coordinates of the head
Coords Snake::info_head()
{ 
    return _body_coords.front(); 
}   

// Setting the coordinates of the snake's head
void Snake::set_head_coords(int x, int y)
{ 
    _body_coords.front().x = x; 
    _body_coords.front().y = y; 
}

// Getting the length of the snake
int Snake::get_snake_len()
{ 
    return _body_coords.size() - 1;
}

// Pointer to the coordinates of the snake
std::list<Coords> &Snake::get_body_coords()
{ 
    return _body_coords; 
}

// Get the direction of movement
int Snake::get_vector()
{ 
    return _head_vector; 
}
