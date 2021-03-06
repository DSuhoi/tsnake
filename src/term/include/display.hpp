/*** * * * * * * * * * * * * * * * * * * * * * ***
 * display.h                                     *
 * This library is needed to simplify interaction*
 * between the game components and the terminal  *
 * screen. The update and delete Window functions*
 * are often used to update, delete, and clear   *
 * Windows. To transfer the program to Windows   *
 * or Mac, you need to edit this file.            *
 *                                               *
 * Created by DSuhoi (2020) (C++17)              *
 *** * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <iostream>
#include <ncurses.h>

// Enumerations for changing the color of a character
enum COLORS {
    GREEN = 1, 
    YELLOW, 
    RED, 
    BLUE 
};

// Display class
struct Display
{
    Display() = delete;
    static void update(WINDOW *win);           // Update the window
    static void update();                      // Updating the entire screen
    static void delete_window(WINDOW *win);    // Deleting the window
    
    static void init_color();                                                // Initializing primary colors
    static void print_scr(WINDOW *win, int x, int y, chtype ch);             // Print the character
    static void print_scr(WINDOW *win, int x, int y, char *buff);            // Print the text
    static void print_scr(WINDOW *win, int x, int y, char *buff, int color); // Print the color text 
};

#endif
