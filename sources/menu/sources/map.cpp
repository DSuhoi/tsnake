#include <iostream>
#include "../map.h"

// Defining Map class fields
int Map::height = 0;        
int Map::width = 0;
WINDOW *Map::map = nullptr;
Coords *Map::borders = nullptr;
Coords *Map::fruits = nullptr;
Coords Map::spawnSnake = {3, 3};
unsigned int Map::numFruits = 0;
unsigned int Map::numBorder = 0;


// Configure the map
void Map::initMap()
{
    int screenWidth, screenHeight;
    getmaxyx(stdscr, screenHeight, screenWidth);        
    // Creating a map window
    map = newwin(HEIGHT + 2, WIDTH, (screenHeight - (HEIGHT + 2))/2, (screenWidth - WIDTH)/2);
}

// Selecting the map size
void Map::selectSizeMap(int select)
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
    Display::printScr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
}

// Erase the map parametrs
void Map::eraseMap()
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
    spawnSnake = {3, 3};
    numFruits = 0; // Reset the number of fruits and obstacles
    numBorder = 0;
    // Update the map window
    Display::update(map);
}

// Setting the number of fruits
void Map::initFruitCoords(int number)
{
    // Checking number of fruits
    if (0 < number && number < 100)
        numFruits = number; 
    else
        numFruits = 1;
    
    fruits = new Coords[numFruits]; // Allocate memory for the coordinates of the fruit
    std::srand(unsigned(std::time(0))); // Generating numbers from the date
    Coords randomCoords;    // Coordinates of the fruit
    for (unsigned int i = 0; i < numFruits; ++i) {
        do {     
            randomCoords.x = std::rand()% width;    // Random coordinates
            randomCoords.y = std::rand()% height;
        } while (randomCoords.x < 1 || randomCoords.y < 1 ||
            isFruit(randomCoords) || isBorder(randomCoords));
        fruits[i] = randomCoords;   // Checking and assigning coordinates
        setMap(fruits[i].x,fruits[i].y,FRUITCHR);   // Setting the fruit on the map
    }
}

// Setting the borders
void Map::initBorderCoords(Coords snakeCoords)
{
    numBorder = (height * width)/20;    // Number of borders
    
    borders = new Coords[numBorder];    // Allocating memory for the coordinates of obstacles
    
    std::srand(unsigned(std::time(0))); // Generating numbers from the date
    Coords randomCoords;
    for (unsigned int i = 0; i < numBorder; ++i) { 
        do {
            randomCoords.x = std::rand()% width;    // Generate and check the coordinates
            randomCoords.y = std::rand()% height;   // No closer than 5 blocks to the initial position of the snake
        } while (isBorder(randomCoords) || randomCoords.x < 1 || randomCoords.y < 1 ||
          ((snakeCoords.x - 3) < randomCoords.x  && randomCoords.x < (snakeCoords.x + 5) && 
            randomCoords.y == snakeCoords.y ));
        borders[i] = randomCoords;  // Assigning and displaying obstacles by coordinates
    }
}

// Creating fruits on the map
void Map::setFruitOnMap(Coords fruitCoords, Coords *snakeCoords, int snakeLen)
{   
    std::srand(unsigned(std::time(0))); // Generate numbers at a time
    int errorCounter = 0;   // Repeat count
    Coords randomCoords;
    for (unsigned int set = 0; set < numFruits; ++set)
        if (fruitCoords == fruits[set]) {
            do { 
                if (errorCounter++ > 1000) return; // If more than 1000 repetitions have passed, we exit the loop
                randomCoords.x = std::rand()% width;    // Random coordinates
                randomCoords.y = std::rand()% height;
                // Check the coordinates
            } while (isSnake(randomCoords, snakeCoords, snakeLen) || 
                    randomCoords.x < 1 || randomCoords.y < 1 || 
                    isFruit(randomCoords) || isBorder(randomCoords));
            fruits[set] = randomCoords; // Assigning the correct coordinates
            setMap(fruits[set].x,fruits[set].y,FRUITCHR);   // and fruit output
        }
}

// Copy the coordinates
void Map::borderCoordsCpy(Coords *borderCoords, int numCoords, Coords spawnCoords)
{
    numBorder = numCoords;  // If the number of coordinates is greater than the length of the array
    if (borders == nullptr)
        borders = new Coords[numBorder];
    
    for (unsigned int i = 0; i < numBorder; ++i)
        borders[i] = borderCoords[i];
    
    spawnSnake = spawnCoords;
}

// Update the images of all objects on the map
void Map::updateMap(Coords *snakeCoords, int snakeLen)
{
    // Updating map borders
    for (int i = 0; i <= width; ++i) {
        setMap(i, 0, BORDERCHR);
        setMap(i, height, BORDERCHR); 
    }
    
    for (int i = 0; i <= height; ++i){
        setMap(0, i, BORDERCHR);
        setMap(width, i, BORDERCHR); 
    }
    // Updating other borders
    for (unsigned int i = 0; i < numBorder; ++i) {
        setMap(borders[i].x, borders[i].y, BORDERCHR);
    }
    // Updating fruits
    for (unsigned int i = 0; i < numFruits; ++i) {
        setMap(fruits[i].x, fruits[i].y, FRUITCHR);
    }
    // Cleaning the snake's tail
    setMap(snakeCoords[snakeLen].x, snakeCoords[snakeLen].y, EMPTYCHR);
    // Updating snake body
    for (unsigned int i = snakeLen; i > 0; --i)
        setMap(snakeCoords[i-1].x, snakeCoords[i-1].y, BODYCHR);
    // Put the character of the head
    setMap(snakeCoords[0].x, snakeCoords[0].y, HEAD);
}

// Checking the player's coordinates
bool Map::isSnake(Coords coords, Coords *snakeCoords, int snakeLen)
{
    for (unsigned int i = snakeLen; i > 0; --i)
        if (coords == snakeCoords[i])
            return true;  
    return false;
}

// Check the coordinates of the fruits
bool Map::isFruit(Coords coords)
{
    for (unsigned int i = 0; i < numFruits; ++i)
        if (coords == fruits[i])
            return true;
    return false;
}

// Check the coordinates of the borders
bool Map::isBorder(Coords coords)
{
    // Checking the pointer
    if (borders == nullptr)
        return false;
    
    for (unsigned int i = 0; i < numBorder; ++i)
        if (coords == borders[i])
            return true;
    return false;
}

// Print the static part of the submenu
void Map::printSubMenuStatic(const long lastScore, const int level)
{
    char buffLastScore[15]; // Array for the record
    char buffLevel[10];     // Array for the level
    sprintf(buffLastScore,"Record: %0*ld", 6, lastScore);
    sprintf(buffLevel,"Level: %d", level);
    Display::printScr(map, WIDTH/2 - 18, HEIGHT, buffLastScore, RED);
    Display::printScr(map, WIDTH/2 + 5, HEIGHT, buffLevel,GREEN);
}

// Print the dynamic part of the submenu
void Map::printSubMenuActive(const long score, time_t &firstTime)
{
    int allTime = time(0) - firstTime;     // All the time since the game started
    unsigned int sec = allTime % 60;       // Seconds
    unsigned int min = allTime / 60;       // Minuts
    char buffScore[14];                    // Array for the score
    char buffTime[15];                     // Array for the time
    sprintf(buffScore,"Score: %0*ld", 6, score);
    sprintf(buffTime,"Time: %0*d:%0*d", 2, min, 2, sec);
    Display::printScr(map, 2, HEIGHT, buffScore,YELLOW);
    Display::printScr(map, WIDTH - 15, HEIGHT, buffTime,BLUE);
}

// Set character on the map
void Map::setMap(int x, int y, chtype color)
{
    Display::printScr(map,((WIDTH - width)/2) + x, ((HEIGHT - height)/2) + y, color);
}

// Get the coordinates of the snake spawn
Coords Map::getSpawnSnake()
{ 
    return spawnSnake; 
}

// Return the map height
int Map::getHeight()
{ 
    return height; 
}

// Return the map width
int Map::getWidth()
{ 
    return width; 
}
