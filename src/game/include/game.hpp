/*** * * * * * * * * * * * * * * * ***
 * game.h                            *
 * The Game class that combines other*
 * Map, Menu, and Snake classes.     *
 * It implements initialization of   *
 * all components and their removal, *
 * game state processing, and output *
 * of results.                       *
 *                                   *
 * Created by DSuhoi (2020) (C++11)  *
 *** * * * * * * * * * * * * * * * ***/


#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "snake.hpp"
#include "map.hpp"
#include "menu.hpp"
#include <ctime>    // Library for working with time

// Game status flag
enum STATUS_GAME {
    GAME_WIN = 0,
    GAME_NOT_WIN,
    GAME_RESTART,
    RETURN_MENU,
    GAME_END
};

// Class of game
class Game {
private: 
    static Snake *snake;            // Pointer to the snake object
    static time_t gameTime;         // Start time of the game
    static long gameScore[30];      // Last records
    
    Game(){}                        // Constructor
    static void startGame();        // Function for configuring the field
    static bool checkWin();         // Check for fail
    static int genScore(int level); // Generating the score
public:
    static void start();            // Function for initializing components
    static void process();          // Game process (main logic)
    static void endGame();          // End of the game
    
};

#endif
