/*** * * * * * * * * * * * * * * * ***
 * game.h                            *
 * The Game class that combines other*
 * Map, Menu, and Snake classes.     *
 * It implements initialization of   *
 * all components and their removal, *
 * game state processing, and output *
 * of results.                       *
 *                                   *
 * Created by DSuhoi (2020) (C++17)  *
 *** * * * * * * * * * * * * * * * ***/


#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <iostream> // Library for working with time
#include "map.hpp"
#include "menu.hpp"
#include "snake.hpp"


// Game status flag
enum STATUS_GAME {
    GAME_WIN = 0,
    GAME_NOT_WIN,
    GAME_RESTART,
    RETURN_MENU,
    GAME_END
};

// Class of game
class Game
{
public:
    static void start();             // Function for initializing components
    static void process();           // Game process (main logic)
    static void end_game();          // End of the game
private: 
    static Snake *snake;             // Pointer to the snake object
    static time_t game_time;         // Start time of the game
    static long game_score[30];      // Last records
    
    Game() = delete;                 // Constructor
    static void start_game();        // Function for configuring the field
    static bool check_win();         // Check for fail
    static int gen_score(int level); // Generating the score
};

#endif
