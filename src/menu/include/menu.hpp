/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * menu.h                                                          *
 * The Menu class contains pointers to Windows, as well as         *
 * control settings and maps. The class contains methods that are  *
 * responsible for the settings and information  menu. As the game *
 * progresses, this file will be updated with new elements.         *
 *                                                                 *
 * Created by DSuhoi (2020) (C++11)                                *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/

#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <cstring>
#include "display.hpp"
#include "periph.hpp"
#include "files.hpp"

// Dimensions:

// Main menu
const int MENU_HEIGHT = 12;
const int MENU_WIDTH = 20;

// Help menu
const int HELP_HEIGHT = 10;
const int HELP_WIDTH = 35;
// Shift in this menu to set items on the right side
const int Help_WIDTH_RIGHT = 22;

// Pause menu
const int PAUSE_HEIGHT = 6;
const int PAUSE_WIDTH = 18;

// Info menu
const int INFO_HEIGHT = 4;
const int INFO_WIDTH = 20;

// Class of menu
class Menu
{
public:
    static void init_main_menu();         // Initializing the main menu
    static int main_menu_loop();          // Main menu loop
    static void help_loop();              // Help loop
    static int pause_loop();              // Pause loop
    static bool print_info(int width, int height, char *string_with_info, bool select);    // Print information in a separate window
    static CONFIG& get_config_map();      // Return the map settings
    static int *get_control();            // The return of the control settings
private:
    static WINDOW *menu_window;           // Pointer to the main menu
    static WINDOW *info_window;           // Pointer to the info menu
    static int buttons[4];                // Control settings
    static CONFIG config_map;             // Map settings

    Menu() = delete;                      // Constructor
    static void lvl_settings_loop();      // Settings menu of the game
    static void control_settings_loop();  // Settings menu of the control
};

#endif
