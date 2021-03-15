/*** * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * periph.h                                                *
 * The library with functions for initializing, disabling, *
 * and running the keyboard handler.                       *
 *                                                         *
 * Created by DSuhoi (2020) (C++17)                        *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __PERIPH_HPP__
#define __PERIPH_HPP__

#include <ncurses.h>
#include <thread>     // Library for game_delay() function
#include <chrono>
#include "menu.hpp"

constexpr auto  SLEEP_TIME = std::chrono::milliseconds(10);   // Minimum delay
constexpr int CURS_KEY_MIN = 258;   // Number (scan code) of the "down" cursor key"
constexpr int CURS_KEY_MAX = 261;   // Number (scan code) of the "right" cursor key"

enum CONTROL {
    DOWN = 0, 
    UP, 
    LEFT, 
    RIGHT
};

// Peripherals class
struct Periph
{
    Periph() = delete;
    static void game_delay(int delay_time);    // Function-manual delay wrapper
    static void init_periph();                 // Initializing the screen and key processing
    static void erase_periph();                // End of key processing functions
    
    static int get_button();                                       // Button processing
    static int get_button(int *game_buttons, float pause_delay);   // Processing of game buttons
};



#endif
