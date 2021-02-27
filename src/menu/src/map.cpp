#include <iostream>
#include "map.hpp"

// Defining Map class fields
int Map::height = 0;        
int Map::width = 0;
WINDOW *Map::map = nullptr;
Coords *Map::borders = nullptr;
Coords *Map::fruits = nullptr;
Coords Map::spawn_snake = {3, 3};
unsigned int Map::num_fruits = 0;
unsigned int Map::num_border = 0;


// Configure the map
void Map::init_map()
{
    int screen_width, screen_height;
    getmaxyx(stdscr, screen_height, screen_width);        
    // Creating a map window
    map = newwin(HEIGHT + 2, WIDTH, (screen_height - (HEIGHT + 2))/2, (screen_width - WIDTH)/2);
}

// Selecting the map size
void Map::select_size_map(int select)
{
    Display::update(map);       // Update the map window
    switch (select) {            // Selecting the map size
    case 0: 
        width = SMALL_WIDTH; 
        height = SMALL_HEIGHT; 
        break;
    case 2: 
        width = BIG_WIDTH; 
        height = BIG_HEIGHT; 
        break;
    case 1: 
    default: 
        width = MEDIUM_WIDTH; 
        height = MEDIUM_HEIGHT; 
        break;
    };
    
    // Print the title
    Display::print_scr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
}

// Erase the map parametrs
void Map::erase_map()
{
    // Checking pointers and freeing up memory
    if (fruits != nullptr) { 
        delete [] fruits; 
        fruits = nullptr; 
    }
    
    if (borders != nullptr) { 
        delete [] borders; 
        borders = nullptr; 
    }
    
    width = 0;  // Resetting the width and height of the field
    height = 0;
    spawn_snake = {3, 3};
    num_fruits = 0; // Reset the number of fruits and obstacles
    num_border = 0;
    // Update the map window
    Display::update(map);
}

// Setting the number of fruits
void Map::init_fruit_coords(int number)
{
    // Checking number of fruits
    if (0 < number && number < 100)
        num_fruits = number; 
    else
        num_fruits = 1;
    
    fruits = new Coords[num_fruits]; // Allocate memory for the coordinates of the fruit
    std::srand(unsigned(std::time(0))); // Generating numbers from the date
    Coords random_coords;    // Coordinates of the fruit
    for (unsigned int i = 0; i < num_fruits; ++i) {
        do {     
            random_coords.x = std::rand()% width;    // Random coordinates
            random_coords.y = std::rand()% height;
        } while (random_coords.x < 1 || random_coords.y < 1 ||
            is_fruit(random_coords) || is_border(random_coords));
        fruits[i] = random_coords;   // Checking and assigning coordinates
        set_map(fruits[i].x,fruits[i].y,FRUITCHR);   // Setting the fruit on the map
    }
}

// Setting the borders
void Map::init_border_coords(Coords snake_coords)
{
    num_border = (height * width)/20;    // Number of borders
    
    borders = new Coords[num_border];    // Allocating memory for the coordinates of obstacles
    
    std::srand(unsigned(std::time(0))); // Generating numbers from the date
    Coords random_coords;
    for (unsigned int i = 0; i < num_border; ++i) { 
        do {
            random_coords.x = std::rand()% width;    // Generate and check the coordinates
            random_coords.y = std::rand()% height;   // No closer than 5 blocks to the initial position of the snake
        } while (is_border(random_coords) || random_coords.x < 1 || random_coords.y < 1 ||
          ((snake_coords.x - 3) < random_coords.x  && random_coords.x < (snake_coords.x + 5) && 
            random_coords.y == snake_coords.y ));
        borders[i] = random_coords;  // Assigning and displaying obstacles by coordinates
    }
}

// Creating fruits on the map
void Map::set_fruit_on_map(Coords fruit_coords, Coords *snake_coords, int snake_len)
{   
    std::srand(unsigned(std::time(0))); // Generate numbers at a time
    int error_counter = 0;   // Repeat count
    Coords random_coords;
    for (unsigned int set = 0; set < num_fruits; ++set)
        if (fruit_coords == fruits[set]) {
            do { 
                if (error_counter++ > 1000) return; // If more than 1000 repetitions have passed, we exit the loop
                random_coords.x = std::rand()% width;    // Random coordinates
                random_coords.y = std::rand()% height;
                // Check the coordinates
            } while (is_snake(random_coords, snake_coords, snake_len) || 
                    random_coords.x < 1 || random_coords.y < 1 || 
                    is_fruit(random_coords) || is_border(random_coords));
            fruits[set] = random_coords; // Assigning the correct coordinates
            set_map(fruits[set].x,fruits[set].y,FRUITCHR);   // and fruit output
        }
}

// Copy the coordinates
void Map::border_coords_cpy(Coords *border_coords, int num_coords, Coords spawn_coords)
{
    num_border = num_coords;  // If the number of coordinates is greater than the length of the array
    if (borders == nullptr)
        borders = new Coords[num_border];
    
    for (unsigned int i = 0; i < num_border; ++i)
        borders[i] = border_coords[i];
    
    spawn_snake = spawn_coords;
}

// Update the images of all objects on the map
void Map::update_map(Coords *snake_coords, int snake_len)
{
    // Updating map borders
    for (int i = 0; i <= width; ++i) {
        set_map(i, 0, BORDERCHR);
        set_map(i, height, BORDERCHR); 
    }
    
    for (int i = 0; i <= height; ++i){
        set_map(0, i, BORDERCHR);
        set_map(width, i, BORDERCHR); 
    }
    // Updating other borders
    for (unsigned int i = 0; i < num_border; ++i) {
        set_map(borders[i].x, borders[i].y, BORDERCHR);
    }
    // Updating fruits
    for (unsigned int i = 0; i < num_fruits; ++i) {
        set_map(fruits[i].x, fruits[i].y, FRUITCHR);
    }
    // Cleaning the snake's tail
    set_map(snake_coords[snake_len].x, snake_coords[snake_len].y, EMPTYCHR);
    // Updating snake body
    for (unsigned int i = snake_len; i > 0; --i)
        set_map(snake_coords[i-1].x, snake_coords[i-1].y, BODYCHR);
    // Put the character of the head
    set_map(snake_coords[0].x, snake_coords[0].y, HEAD);
}

// Checking the player's coordinates
bool Map::is_snake(Coords coords, Coords *snake_coords, int snake_len)
{
    for (unsigned int i = snake_len; i > 0; --i)
        if (coords == snake_coords[i])
            return true;  
    return false;
}

// Check the coordinates of the fruits
bool Map::is_fruit(Coords coords)
{
    for (unsigned int i = 0; i < num_fruits; ++i)
        if (coords == fruits[i])
            return true;
    return false;
}

// Check the coordinates of the borders
bool Map::is_border(Coords coords)
{
    // Checking the pointer
    if (borders == nullptr)
        return false;
    
    for (unsigned int i = 0; i < num_border; ++i)
        if (coords == borders[i])
            return true;
    return false;
}

// Print the static part of the submenu
void Map::print_sub_menu_static(const long last_score, const int level)
{
    char buff_last_score[15]; // Array for the record
    char buff_level[10];     // Array for the level
    sprintf(buff_last_score,"Record: %0*ld", 6, last_score);
    sprintf(buff_level,"Level: %d", level);
    Display::print_scr(map, WIDTH/2 - 18, HEIGHT, buff_last_score, RED);
    Display::print_scr(map, WIDTH/2 + 5, HEIGHT, buff_level, GREEN);
}

// Print the dynamic part of the submenu
void Map::print_sub_menu_active(const long score, time_t &first_time)
{
    int all_time = time(0) - first_time;     // All the time since the game started
    unsigned int sec = all_time % 60;       // Seconds
    unsigned int min = all_time / 60;       // Minuts
    char buff_score[14];                    // Array for the score
    char buff_time[15];                     // Array for the time
    sprintf(buff_score,"Score: %0*ld", 6, score);
    sprintf(buff_time,"Time: %0*d:%0*d", 2, min, 2, sec);
    Display::print_scr(map, 2, HEIGHT, buff_score, YELLOW);
    Display::print_scr(map, WIDTH - 15, HEIGHT, buff_time, BLUE);
}

// Set character on the map
void Map::set_map(int x, int y, chtype color)
{
    Display::print_scr(map,((WIDTH - width)/2) + x, ((HEIGHT - height)/2) + y, color);
}

// Get the coordinates of the snake spawn
Coords Map::get_spawn_snake()
{ 
    return spawn_snake; 
}

// Return the map height
int Map::get_height()
{ 
    return height; 
}

// Return the map width
int Map::get_width()
{ 
    return width; 
}
