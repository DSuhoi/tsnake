#include <iostream>
#include "map.h"

// Defining Map class fields
int Map::height = 0;		
int Map::width = 0;
WINDOW *Map::map = nullptr;
Coords *Map::borders = nullptr;
Coords *Map::fruits = nullptr;
Coords Map::spawnSnake = {3, 3};
int Map::numFruits = 0;
int Map::numBorder = 0;


// Configure the map
void Map::InitMap()
{
	int screenWidth, screenHeight;
	getmaxyx(stdscr, screenHeight, screenWidth);		
	// Creating a map window
	map = newwin(HEIGHT + 2, WIDTH, (screenHeight - (HEIGHT + 2))/2, (screenWidth - WIDTH)/2);
}

// Selecting the map size
void Map::SelectSizeMap(int select)
{
	Display::Update(map);		// Update the map window
	switch(select){				// Selecting the map size
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
	Display::PrintScr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
}

// Erase the map parametrs
void Map::EraseMap()
{
	// Checking pointers and freeing up memory
	if(fruits != nullptr){ 
		delete [] fruits; 
		fruits = nullptr; 
	}
	
	if(borders != nullptr){ 
		delete [] borders; 
		borders = nullptr; 
	}
	
	width = 0;	// Resetting the width and height of the field
	height = 0;
	spawnSnake = {3, 3};
	numFruits = 0; // Reset the number of fruits and obstacles
	numBorder = 0;
	// Update the map window
	Display::Update(map);
}

// Setting the number of fruits
void Map::InitFruitCoords(int number)
{
	// Checking number of fruits
	if(0 < number && number < 100){ 
		numFruits = number;	
	}
	else{ 
		numFruits = 1;
	}
	
	fruits = new Coords[numFruits];	// Allocate memory for the coordinates of the fruit
	std::srand(unsigned(std::time(0)));	// Generating numbers from the date
	Coords randomCoords;	// Coordinates of the fruit
	for(int i = 0; i < numFruits; i++){
		do{	 
			randomCoords.x = std::rand()% width;	// Random coordinates
			randomCoords.y = std::rand()% height;
		}while(randomCoords.x < 1 || randomCoords.y < 1 ||
			IsFruit(randomCoords) || IsBorder(randomCoords));
		fruits[i] = randomCoords;	// Checking and assigning coordinates
		SetMap(fruits[i].x,fruits[i].y,FRUITCHR);	// Setting the fruit on the map
	}
}

// Setting the borders
void Map::InitBorderCoords(Coords snakeCoords)
{
	numBorder = (height * width)/20;	// Number of borders
	
	borders = new Coords[numBorder];	// Allocating memory for the coordinates of obstacles
	
	std::srand(unsigned(std::time(0)));	// Generating numbers from the date
	Coords randomCoords;
	for(int i = 0; i < numBorder; i++){	
		do{
			randomCoords.x = std::rand()% width;	// Generate and check the coordinates
			randomCoords.y = std::rand()% height;	// No closer than 5 blocks to the initial position of the snake
	   }while(IsBorder(randomCoords) || 
		((snakeCoords.x - 3) < randomCoords.x  && randomCoords.x < (snakeCoords.x + 5) && 
		randomCoords.y == snakeCoords.y ) || randomCoords.x < 1 || randomCoords.y < 1);
		borders[i] = randomCoords;	// Assigning and displaying obstacles by coordinates
	}
}

// Creating fruits on the map
void Map::SetFruitOnMap(Coords fruitCoords, Coords *snakeCoords, int snakeLen)
{	
	std::srand(unsigned(std::time(0)));	// Generate numbers at a time
	int errorCounter = 0;	// Repeat count
	Coords randomCoords;
	for(int set = 0; set < numFruits;set++){
		if(fruitCoords == fruits[set]){
			do{	
				if(errorCounter++ > 1000) return; // If more than 1000 repetitions have passed, we exit the loop
				randomCoords.x = std::rand()% width;	// Random coordinates
				randomCoords.y = std::rand()% height;
				// Check the coordinates
			}while(IsSnake(randomCoords, snakeCoords, snakeLen) || 
					randomCoords.x < 1 || randomCoords.y < 1 || 
						IsFruit(randomCoords) || IsBorder(randomCoords));
			fruits[set] = randomCoords;	// Assigning the correct coordinates
			SetMap(fruits[set].x,fruits[set].y,FRUITCHR);	// and fruit output
		}
	}
}

// Copy the coordinates
void Map::BorderCoordsCpy(Coords *borderCoords, int numCoords, Coords spawnCoords)
{
	numBorder = numCoords;	// If the number of coordinates is greater than the length of the array
	if(borders == nullptr){
		borders = new Coords[numBorder];
	}
	
	for(int i = 0; i < numBorder; i++){
		borders[i] = borderCoords[i];
	}
	
	spawnSnake = spawnCoords;
}

// Update the images of all objects on the map
void Map::UpdateMap(Coords *snakeCoords, int snakeLen)
{
	// Updating map borders
	for(int i = 0; i <= width;i++){
		SetMap(i, 0, BORDERCHR);
		SetMap(i, height, BORDERCHR); 
	}
	
	for(int i = 0; i <= height;i++){
		SetMap(0, i, BORDERCHR);
		SetMap(width, i, BORDERCHR); 
	}
	// Updating other borders
	for(int i = 0; i < numBorder; i++){
		SetMap(borders[i].x, borders[i].y, BORDERCHR);
	}
	// Updating fruits
	for(int i = 0; i < numFruits; i++){
		SetMap(fruits[i].x, fruits[i].y, FRUITCHR);
	}
	
	SetMap(snakeCoords[snakeLen].x, snakeCoords[snakeLen].y, EMPTYCHR);	// Cleaning the snake's tail
	// Updating snake body
	for(int i = snakeLen; i > 0; i--){
		SetMap(snakeCoords[i-1].x, snakeCoords[i-1].y, BODYCHR);
	}
	
	SetMap(snakeCoords[0].x, snakeCoords[0].y, HEAD);	// Put the character of the head
}

// Checking the player's coordinates
bool Map::IsSnake(Coords coords, Coords *snakeCoords, int snakeLen)
{
	for(int i = snakeLen; i > 0; i--){
		if(coords == snakeCoords[i]){
			return true;
		}
	}
	
	return false;
}

// Check the coordinates of the fruits
bool Map::IsFruit(Coords coords)
{
	for(int i = 0; i < numFruits; i++){
		if(coords == fruits[i]){
			return true;
		}
	}
	
	return false;
}

// Check the coordinates of the borders
bool Map::IsBorder(Coords coords)
{
	// Checking the pointer
	if(borders == nullptr){
		return false;
	}
	
	for(int i = 0; i < numBorder; i++){
		if(coords == borders[i]){ 
			return true;
		}
	}
	
	return false;
}

// Print the static part of the submenu
void Map::PrintSubMenuStatic(const long lastScore, const int level)
{
	char buffLastScore[15];	// Array for the record
	char buffLevel[10];		// Array for the level
	sprintf(buffLastScore,"Record: %0*ld", 6, lastScore);
	sprintf(buffLevel,"Level: %d", level);
	Display::PrintScr(map, WIDTH/2 - 18, HEIGHT, buffLastScore, RED);
	Display::PrintScr(map, WIDTH/2 + 5, HEIGHT, buffLevel,GREEN);
}

// Print the dynamic part of the submenu
void Map::PrintSubMenuActive(const long score, time_t &firstTime)
{
	int allTime = time(0) - firstTime;	// All the time since the game started
	unsigned int sec = allTime % 60;		// Seconds
	unsigned int min = allTime / 60;		// Minuts
	char buffScore[14];	// Array for the score
	char buffTime[15];	// Array for the time
	sprintf(buffScore,"Score: %0*ld", 6, score);
	sprintf(buffTime,"Time: %0*d:%0*d", 2, min, 2, sec);
	Display::PrintScr(map, 2, HEIGHT, buffScore,YELLOW);
	Display::PrintScr(map, WIDTH - 15, HEIGHT, buffTime,BLUE);
}

// Set character on the map
void Map::SetMap(int x, int y, chtype color)
{
	Display::PrintScr(map,((WIDTH - width)/2) + x, ((HEIGHT - height)/2) + y, color);
}

// Get the coordinates of the snake spawn
Coords Map::GetSpawnSnake()
{ 
	return spawnSnake; 
}

// Return the map height
int Map::GetHeight()
{ 
	return height; 
}

// Return the map width
int Map::GetWidth()
{ 
	return width; 
}
