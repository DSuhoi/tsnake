#include "map.hpp"
#include <iterator>


Map::Map(int select_map_size, int number_fruits) : 
    Term_window(HEIGHT, WIDTH, (SCREEN_HEIGHT - HEIGHT)/2, (SCREEN_WIDTH - WIDTH)/2) 
{
    spawn_snake = {3, 3};
    select_size_map(select_map_size);
    init_fruit_coords(number_fruits);
}

Map::~Map()
{
    fruits.clear();
    borders.clear(); 
}

// Selecting the map size
void Map::select_size_map(int select)
{
    Display::update(map);       // Update the map window
    switch (select) {           // Selecting the map size
    case 0: 
        width = SMALL_WIDTH; 
        height = SMALL_HEIGHT; 
        break;
    case 2: 
        width = BIG_WIDTH; 
        height = BIG_HEIGHT; 
        break; 
    default: 
        width = MEDIUM_WIDTH; 
        height = MEDIUM_HEIGHT; 
        break;
    };
    
    // Print the title
    print_label(WIDTH - 9, "TSNAKE", BLUE);
}

// Setting the number of fruits
void Map::init_fruit_coords(int number_fruits)
{
    // Checking number of fruits
    if (number_fruits < 1 ||  99 < number_fruits)
        number_fruits = 1;
    
    for (auto i = 0; i < number_fruits; ++i) {
        Coords random_coords;    // Coordinates of the fruit
        do {     
            random_coords.x = 1 + std::rand() % (width - 1);    // Random coordinates
            random_coords.y = 1 + std::rand() % (height - 1);
        } while (is_fruit(random_coords) || is_border(random_coords));
        fruits.push_back(random_coords);   // Checking and assigning coordinates
        map.print(random_coords.y, random_coords.x, FRUITCHR);   // Setting the fruit on the map
    }
}

// Setting the borders
void Map::init_border_coords(Coords snake_coords)
{
    auto number_borders = (height * width)/20;    // Number of borders
    
    for (unsigned int i = 0; i < number_borders; ++i) {
        Coords random_coords;
        do {
            random_coords.x = 1 + std::rand() % (width - 1);    // Generate and check the coordinates
            random_coords.y = 1 + std::rand() % (height - 1);   // No closer than 5 blocks to the initial position of the snake
        } while (is_border(random_coords) ||
               ((snake_coords.x - 3) < random_coords.x  && random_coords.x < (snake_coords.x + 5) && 
                 random_coords.y == snake_coords.y ));
        borders.push_back(random_coords);  // Assigning and displaying obstacles by coordinates
    }
}

// Creating fruits on the map
void Map::set_fruit_on_map(Coords fruit_coords, std::list<Coords> &snake_coords)
{   
    int error_counter = 0;   // Repeat count
    
    for (auto &fruit : fruits)
        Coords random_coords;
        if (fruit_coords == fruit) {
            do { 
                // If more than 1000 repetitions have passed, we exit the loop
                if (error_counter++ > 1000) 
                    return; 
                random_coords.x = 1 + std::rand() % (width - 1);    // Random coordinates
                random_coords.y = 1 + std::rand() % (height - 1);
                // Check the coordinates
            } while (is_snake_tail(random_coords, snake_coords) || 
                     is_fruit(random_coords) || is_border(random_coords));
            fruit = random_coords; // Assigning the correct coordinates
            map.print(it->y, it->x, FRUITCHR);   // and fruit output
        } 
    }
}

// Update the images of all objects on the map
void Map::update_map(std::list<Coords> &snake_coords)
{
    // Updating map borders
    for (auto i = 0; i <= width; ++i) {
        set_map(0, i, BORDERCHR);
        set_map(height, i, BORDERCHR); 
    }
    
    for (auto i = 0; i <= height; ++i) {
        set_map(i, 0, BORDERCHR);
        set_map(i, width, BORDERCHR); 
    }

    // Updating other borders
    for (auto it = borders.cbegin(); it != borders.cend(); ++it)
        set_map(it->y, it->x, BORDERCHR);

    // Updating fruits
    for (auto fruit : fruits)
        set_map(fruit.y, fruit.x, FRUITCHR);

    // Cleaning the snake's tail
    set_map(snake_coords.back().y, snake_coords.back().x, EMPTYCHR);
    // Updating snake body
    for (auto it = (++snake_coords.crbegin()); it != (--snake_coords.crend()); ++it)
        set_map(it->y, it->x, BODYCHR);
    // Put the character of the head
    set_map(snake_coords.front().y, snake_coords.front().x, HEAD);
}

// Checking the player's coordinates
bool Map::is_snake_tail(Coords coords, std::list<Coords> &snake_coords)
{
    for (auto it = (++snake_coords.cbegin()); it != snake_coords.cend(); ++it)
        if (coords == *it)
            return true;
    return false;
}

// Check the coordinates of the fruits
bool Map::is_fruit(Coords coords)
{
    for (auto fruit : fruits)
        if (coords == fruit)
            return true;
    return false;
}

// Check the coordinates of the borders
bool Map::is_border(Coords coords)
{
    for (auto border : borders)
        if (coords == border)
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
    int all_time = time(0) - first_time;    // All the time since the game started
    unsigned int sec = all_time % 60;       // Seconds
    unsigned int min = all_time / 60;       // Minuts
    char buff_score[14];                    // Array for the score
    char buff_time[15];                     // Array for the time
    sprintf(buff_score,"Score: %0*ld", 6, score);
    sprintf(buff_time,"Time: %0*d:%0*d", 2, min, 2, sec);
    Display::print_scr(map, 2, HEIGHT, buff_score, YELLOW);
    Display::print_scr(map, WIDTH - 15, HEIGHT, buff_time, BLUE);
}

// Get the coordinates of the snake spawn
Coords Map::get_spawn_snake()
{ 
    return spawn_snake; 
}

// Return the map height
int Map::get_height()
{ 
    return map.get_height(); 
}

// Return the map width
int Map::get_width()
{ 
    return map.get_width; 
}
