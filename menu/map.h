/*** * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * map.h                                                 *
 * The Map class contains all information about the game *
 * map (size, number of fruits, and borders). This class *
 * implements checking of coordinates and the display of *
 * objects on it.                                        *
 *                                                       *
 * Created by DSuhoi (2020) (C++11)                      *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __MAP_H__
#define __MAP_H__

#include <ncurses.h>
#include <ctime>
#include "../term/coords.h"
#include "../term/display.h"

// Map and fruit characters
const char BORDERCHR = '#';
const chtype FRUITCHR = '$' | COLOR_PAIR(YELLOW);
const chtype EMPTYCHR = ' ';
// Snake characters
const chtype BODYCHR = 'o' | COLOR_PAIR(GREEN);
const chtype HEAD = '@' | COLOR_PAIR(GREEN);
const chtype KILL = 'X' | COLOR_PAIR(RED);

// Width and height of different maps
const int SMALL_WIDTH = 37;
const int SMALL_HEIGHT = 9;

const int MEDIUM_WIDTH = 55;
const int MEDIUM_HEIGHT = 14;

const int BIG_WIDTH = 77;
const int BIG_HEIGHT = 20;

// Width and height of the map window
const int WIDTH = 80;
const int HEIGHT = 22;

// Class of map
class Map {
private:
	static int height;			// Width of the map
	static int width;			// Height of the map
	static WINDOW *map;			// Map window
	static Coords *borders;		// Pointer to the coordinates of the borders
	static Coords *fruits;		// Pointer to the coordinates of the fruits
	static Coords spawnSnake;	// Coordinates of the snake spawn
	static int numFruits;		// number of fruits
	static int numBorder;		// number of borders
	
	Map(){}	// Constructor
	static void borderMap();	// Printing borders
public:

	static void InitMap();			// Configure the map
	static void EraseMap();			// Erase the map parametrs
	static void SelectSizeMap(int select);	// Selecting the map size
	static void InitBorderCoords(Coords snakeCoords);	// Setting the borders
	static void InitFruitCoords(int number);	// Setting the number of fruits
	static void SetFruitOnMap(Coords fruitCoords, Coords *snakeCoords, int snakeLen);	// Creating fruits on the map
	static void UpdateMap(Coords *snake, int snakeLen);	// Update the images of all objects on the map

	static void SetMap(int x, int y, chtype color);	// Set character on the map
	static void PrintSubMenuStatic(const long lastScore, const int level);	// Print the static part of the submenu
	static void PrintSubMenuActive(const long score, time_t &firstTime);	// Print the dynamic part of the submenu
	static void BorderCoordsCpy(Coords *borderCoords, int numCoords, Coords spawnCoords);	// Copy the coordinates
	static int GetHeight();		// Return the map height
	static int GetWidth();		// Return the map width

	static Coords GetSpawnSnake();	// Get the coordinates of the snake spawn
	static bool IsSnake(Coords coords, Coords *snakeCoords, int snakeLen);	// Checking the player's coordinates
	static bool IsFruit(Coords coords);		// Check the coordinates of the fruits
	static bool IsBorder(Coords coords);	// Check the coordinates of the borders
};

#endif
