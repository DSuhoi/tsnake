#include <iostream>
#include <fstream>
#include <cstring>
#include "display.hpp"
#include "menu.hpp"

// Defining Menu class fields
WINDOW *Menu::menu_window;
WINDOW *Menu::info_window;
int Menu::buttons[4];
CONFIG Menu::config_map;


// Initializing the main menu
void Menu::init_main_menu()
{
    int screen_width = 0, screen_height = 0;  // Screen size X*Y
    getmaxyx(stdscr, screen_height, screen_width);
    // Creating the main menu window
    menu_window = newwin(MENU_HEIGHT, MENU_WIDTH,(screen_height - MENU_HEIGHT)/2, (screen_width - MENU_WIDTH)/2);
    // Control and map settings
    buttons[DOWN] = KEY_DOWN;
    buttons[UP] = KEY_UP;
    buttons[LEFT] = KEY_LEFT;
    buttons[RIGHT] = KEY_RIGHT;
    
    config_map = FileSystem::load_settings(buttons);
}

// Main menu loop
int Menu::main_menu_loop()
{
    // Array of menu item names
    std::string menu_part[5] ={"Start Game", "Lvl Settings", "Controls", "Help", "Exit"};
    
    // Clearing and updating the window
    Display::update(menu_window);
    // Selecting a menu elements (highlighting the selected item)
    unsigned int hi_light = 0;    
    while (1) {
        // Output the name of the game
        Display::print_scr(menu_window, MENU_WIDTH/2 - 5, 0, (char*)"TSNAKE 1.3", BLUE);
    
        // Green elements
        for (unsigned int curs_position = 0; curs_position < 5; ++curs_position) {
            if (curs_position == hi_light)   // Highlight the selected line
                wattron(menu_window, COLOR_PAIR(GREEN));
            // Print lines
            Display::print_scr(menu_window, 2, curs_position + 1, (char*)menu_part[curs_position].c_str());
            wattroff(menu_window, COLOR_PAIR(GREEN));
        }
        // Reading the keystroke
        switch (Periph::get_button()) {
        case KEY_UP: 
            if (hi_light > 0)     // Selecting a menu item
                --hi_light;
            break;  
        case KEY_DOWN: 
            if (hi_light < 4)
                ++hi_light; 
            break;
        case KEY_EXIT: 
            return 0;            // Exit the program
            break;      
        case KEY_ENTER:
            switch (hi_light) {
            case 0: 
                return 1;        // If this is a game, then exit the menu
                break;  
            case 1: 
                lvl_settings_loop();      // Level settings
                break;      
            case 2: 
                control_settings_loop();  // Control settings
                break;  
            case 3: 
                help_loop(); // Information
                break;
            case 4: 
                return 0;   // Exit the program
                break;
            }; 
            Display::update(menu_window);
            break;
        };
    }
}

// Settings menu of the game
void Menu::lvl_settings_loop()
{
    // Menu elements, map parameters, and responses
    std::string menu_part[8] = {"Back", "Speed:", "Fruit:","Map Size:", 
        "Border:", "Teleport:", "Erase Settings", "Erase Records"};
    std::string map_size[3] = {"Small", "Medium", "Large"};
    std::string select_str[2] = {"Off", "On"};
    // Character array for displaying menu lines
    char buff_str[10];
    // Updating the menu window 
    Display::update(menu_window);
    // Type of brackets (0 - rectangular, 1-sharp)
    int current_bracket = 0;
    unsigned int hi_light = 0;
    while (1) {
    
        Display::print_scr(menu_window, MENU_WIDTH/2 - 6, 0, (char*)"Lvl Settings", BLUE);
        
        for (unsigned int curs_position = 0; curs_position < 8; ++curs_position) {
            if (curs_position == hi_light) {
                // Enable the green light for the selected element
                wattron(menu_window, COLOR_PAIR(GREEN));
                current_bracket = 1;
            } else { 
                current_bracket = 0;
            }
            
            // Defining a parameter based on the position of the menu rendering cursor
            char parametr[7] = "";
            switch (curs_position) {
            case 1:
                sprintf(parametr, "%d", config_map.speed);
                break;
            case 2:
                sprintf(parametr, "%d", config_map.num_fruits);
                break;
            case 3:
                strcpy(parametr, (char*)map_size[config_map.map_size].c_str());
                break;
            case 4:
                strcpy(parametr, (char*)select_str[config_map.border].c_str());
                break;
            case 5:
                strcpy(parametr, (char*)select_str[config_map.teleport].c_str());
                break;
            };
            
            if (curs_position == 0) {
                // Print the exit parameter from the menu
                Display::print_scr(menu_window, 2, 1, (char*)menu_part[0].c_str());
            } else if (curs_position > 5) {
                // Print the parameter for clearing settings
                Display::print_scr(menu_window, 2, (MENU_HEIGHT - 9) + curs_position, (char*)menu_part[curs_position].c_str());
            } else {
                // Left and right brackets
                char bracketL[2] = {'[', '<'};
                char bracketR[2] = {']', '>'};
                // Generating the menu bar
                sprintf(buff_str,"%c%s%c", bracketL[current_bracket], parametr, bracketR[current_bracket]);
                // Displays the parameter name and its value (centered)
                Display::print_scr(menu_window, MENU_WIDTH - strlen(buff_str)/2 - 5, curs_position + 2, buff_str);
                Display::print_scr(menu_window, 2, curs_position + 2, (char*)menu_part[curs_position].c_str());
            }
        
            // Clearing the array
            for(unsigned int index = 0; index < 9; index++)
                buff_str[index] = 0;
            // Disable the green light
            wattroff(menu_window, COLOR_PAIR(GREEN));
        }
        // Processing keystrokes
        switch(Periph::get_button()) {
        case KEY_UP: 
            if (hi_light > 0)
                --hi_light; 
            break;
        case KEY_DOWN: 
            if (hi_light < 7)
                ++hi_light;
            break;
        case KEY_LEFT: // Depending on the element, configure the parameters
            switch(hi_light) {
            case 1: 
                // Speed reduction, number of fruits on the map and its size
                if(config_map.speed > 1)
                    --config_map.speed;
                break;
            case 2: 
                if(config_map.num_fruits > 1)
                    --config_map.num_fruits;
                break;
            case 3: 
                if(config_map.map_size > 0)
                    --config_map.map_size;
                break;
            case 4: 
                // Disabling obstacles
                config_map.border = false; 
                break;
            case 5: 
                // Disables the ability to teleport
                config_map.teleport = false; 
                break;
            };
            // Updating the settings menu
            Display::update(menu_window);
            break;
        case KEY_RIGHT:
            switch(hi_light) {
            case 1: 
                if(config_map.speed < 10)
                    ++config_map.speed; 
                break;
            case 2: 
                if(config_map.num_fruits < 99)
                    ++config_map.num_fruits;
                break;
            case 3: 
                if(config_map.map_size < 2)
                    ++config_map.map_size;
                break;
            case 4: 
                config_map.border = true; 
                break;
            case 5: 
                config_map.teleport = true; 
                break;
            };
            // Updating the settings menu
            Display::update(menu_window);
            break;
        case KEY_EXIT: 
            FileSystem::save_settings(config_map, buttons); 
            return; 
            break;
        case KEY_ENTER: 
            if (hi_light == 0) { 
                // Saving settings in a file when exiting the menu
                FileSystem::save_settings(config_map, buttons); 
                return; 
            } else if (hi_light == 6) {
                // Default settings
                config_map = {8, 1, 1, false, false, false};
                // Updating the settings menu
                Display::update(menu_window); 
            } else if (hi_light == 7) {
                // Displays the menu about clearing records
                if (print_info(INFO_WIDTH - 6, INFO_HEIGHT,(char*)"CLEAR DATA ?", true))
                    config_map.clear_score = true;
            }
            // Update the screen
            Display::update();
            break;
        };
    }
}

// Settings menu of the control
void Menu::control_settings_loop()
{
    // Menu elements and cursor key names (could be done via map container)
    std::string menu_part[7] = {"Back", "Key Down:", "Key Up:", "Key Left:", "Key Right:", "Erase Settings"};
    std::string curs_key[4] = {"down ", "up  ", "left ", "right"};
    
    Display::update(menu_window);
    
    unsigned int hi_light = 0;
    while (1) {
    
        Display::print_scr(menu_window, MENU_WIDTH/2 - 4, 0, (char*)"Controls", BLUE);
    
        for (unsigned int curs_position = 0; curs_position < 6; ++curs_position) {
            // Cursor highlighting
            if (curs_position == hi_light)
                wattron(menu_window, COLOR_PAIR(GREEN));
            // Button display
            if (0 < curs_position && curs_position < 5){
                if (buttons[curs_position - 1] < CURS_KEY_MIN || CURS_KEY_MAX < buttons[curs_position - 1])
                    Display::print_scr(menu_window, MENU_WIDTH - 7, curs_position + 2, buttons[curs_position - 1]);
                else
                    Display::print_scr(menu_window, MENU_WIDTH - 7, curs_position + 2, (char*)curs_key[buttons[curs_position - 1] - CURS_KEY_MIN].c_str()); 
            }
        
            if (curs_position == 0)
                // Print the exit parameter from the menu
                Display::print_scr(menu_window, 2, 1, (char*)menu_part[0].c_str());
            else if (curs_position == 5)
                // Print the parameter for clearing settings
                Display::print_scr(menu_window, 2, MENU_HEIGHT - 3, (char*)menu_part[5].c_str());
            else
                Display::print_scr(menu_window, 2, curs_position + 2, (char*)menu_part[curs_position].c_str());
        
            wattroff(menu_window, COLOR_PAIR(GREEN));
        }
    
        switch (Periph::get_button()) {
        case KEY_UP: 
            if (hi_light > 0)
                --hi_light;
            break;
        case KEY_DOWN: 
            if (hi_light < 5)
                ++hi_light; 
            break;
        case KEY_EXIT:
            // Saving keys in a file when exiting the menu
            FileSystem::save_settings(config_map,buttons); 
            return; 
            break;
        case KEY_ENTER: 
            if (hi_light == 0) {
                // Saving keys in a file when exiting the menu
                FileSystem::save_settings(config_map,buttons); 
                return; 
            } else if (hi_light == 5) {
                // Reset keys
                buttons[DOWN] = KEY_DOWN;
                buttons[UP] = KEY_UP;
                buttons[LEFT] = KEY_LEFT;
                buttons[RIGHT] = KEY_RIGHT;
            } else {
                // Creating an information window for processing the pressed key
                print_info(INFO_WIDTH, INFO_HEIGHT - 1, (char*)"Press the button!", false);
                cbreak();
                nodelay(stdscr, false);         // Disable the delay
                int recieved_button = getch();   // Recieve the symbol
                nodelay(stdscr, true);          // Enable the delay
                Display::delete_window(info_window);   // Delete the window
                // Assign a character
                switch (hi_light) {
                case 1: 
                    buttons[DOWN] = recieved_button; 
                    break;
                case 2: 
                    buttons[UP] = recieved_button; 
                    break;
                case 3: 
                    buttons[LEFT] = recieved_button; 
                    break;
                case 4: 
                    buttons[RIGHT] = recieved_button; 
                    break;
                };
        
            }
            Display::update(menu_window); 
            break;
        };
    
    }
}

// Information
void Menu::help_loop()
{
    int screen_width = 0, screen_height = 0;
    getmaxyx(stdscr, screen_height, screen_width);
    // Creating the info window in the center of the screen
    info_window = newwin(HELP_HEIGHT, HELP_WIDTH, (screen_height - HELP_HEIGHT)/2, (screen_width - HELP_WIDTH)/2);
    Display::update(info_window);
    
    bool select = false;
    while (1) {
        // Print information about control and the game
        Display::print_scr(info_window, HELP_WIDTH - 6, 0, (char*)"HELP", BLUE);
    
        if (!select) {
            Display::print_scr(info_window, 1, 0, (char*)"(1)", BLUE);
            Display::print_scr(info_window, 2, 1, (char*)"Controls:", YELLOW);
            wattron(info_window, COLOR_PAIR(GREEN));
            Display::print_scr(info_window, 2, 2, (char*)"Key Down:");
            Display::print_scr(info_window, 2, 3, (char*)"Key Up:");
            Display::print_scr(info_window, 2, 4, (char*)"Key Left:");
            Display::print_scr(info_window, 2, 5, (char*)"Key Right:");
            Display::print_scr(info_window, 2, 6, (char*)"Key Pause Game:");
            Display::print_scr(info_window, 2, 7, (char*)"Key Help Game:");
            Display::print_scr(info_window, 2, 8, (char*)"Key Quit Game:");
            wattroff(info_window, COLOR_PAIR(GREEN));
            Display::print_scr(info_window, Help_WIDTH_RIGHT, 6, 'p');
            Display::print_scr(info_window, Help_WIDTH_RIGHT, 7, 'h');
            Display::print_scr(info_window, Help_WIDTH_RIGHT, 8, 'q');
    
            // Name of cursor keys
            std::string curs_key[4] = {"down", "up", "left", "right"};
            // Print of control keys
            for (int current_button = 0; current_button < 4; ++current_button)
                if (buttons[current_button] < CURS_KEY_MIN || CURS_KEY_MAX < buttons[current_button])
                    Display::print_scr(info_window, Help_WIDTH_RIGHT, current_button + 2, buttons[current_button]);
                else
                    Display::print_scr(info_window, Help_WIDTH_RIGHT, current_button + 2, (char*)curs_key[buttons[current_button] - CURS_KEY_MIN].c_str());
        } else {
            // Print reference information
            Display::print_scr(info_window, 1, 0, (char*)"(2)", BLUE);
            Display::print_scr(info_window, 2, 1, (char*)"About:", YELLOW);
            Display::print_scr(info_window, 2, 2, (char*)"Game", GREEN);
            Display::print_scr(info_window, 7, 2, 'T' | COLOR_PAIR(GREEN));
            Display::print_scr(info_window, 8, 2, 'S' | COLOR_PAIR(GREEN));
            Display::print_scr(info_window, 9, 2, 'N' | COLOR_PAIR(YELLOW));
            Display::print_scr(info_window, 10, 2, 'A' | COLOR_PAIR(RED));
            Display::print_scr(info_window, 11, 2, 'K' | COLOR_PAIR(GREEN));
            Display::print_scr(info_window, 12, 2, 'E'| COLOR_PAIR(BLUE));
            Display::print_scr(info_window, 14, 2, (char*)"v_1.3", BLUE);
            wattron(info_window, COLOR_PAIR(GREEN));
            Display::print_scr(info_window, 2, 4, (char*)"Game created by DSuhoi (2020)");
            Display::print_scr(info_window, 2, 5, (char*)"Email: <dsuh0i.h8@gmail.com>");
            Display::print_scr(info_window, 2, 6, (char*)"Sourse Code:");
            Display::print_scr(info_window, 2, 7, (char*)"https://github.com/DSuhoi/tsnake");
            Display::print_scr(info_window, 5, 8, (char*)"Thanks for playing!");
            wattroff(info_window, COLOR_PAIR(GREEN));
        }
        switch (Periph::get_button()) {    //Exit the information menu
        case KEY_ENTER:
        case KEY_EXIT:
        case 'h':
            // Deleting the info window
            Display::delete_window(info_window); 
            return; 
            break;
        case KEY_LEFT: 
            select = false; 
            Display::update(info_window); 
            break;
        case KEY_RIGHT: 
            select = true; 
            Display::update(info_window); 
            break;
        };
    }
}

// Pause loop
int Menu::pause_loop()
{
    // Window element
    std::string pause_menu_str[4] = {"Resume", "Restart", "Quit to Menu", "Quit Game"};
    
    int screen_width = 0, screen_height = 0;
    getmaxyx(stdscr, screen_height, screen_width);
    // Create the window
    info_window = newwin(PAUSE_HEIGHT, PAUSE_WIDTH, (screen_height - PAUSE_HEIGHT)/2, (screen_width - PAUSE_WIDTH)/2);
    Display::update(info_window);
    Display::print_scr(info_window, PAUSE_WIDTH - 7, 0, (char*)"PAUSE", BLUE);
    // Menu highlighting
    unsigned int hi_light = 0;
    while (1) {

        for (unsigned int i = 0; i < 4; ++i) {
            if (i == hi_light)
                wattron(info_window, COLOR_PAIR(GREEN));
            Display::print_scr(info_window, 2, i + 1, (char*)pause_menu_str[i].c_str());
            wattroff(info_window, COLOR_PAIR(GREEN));
        }

        switch (Periph::get_button()) {
        case KEY_UP:
            if (hi_light > 0)
                --hi_light;
            break;
        case KEY_DOWN:
            if (hi_light < 3)
                ++hi_light;
            break;
        case KEY_EXIT:
        case 'p':
            Display::delete_window(info_window);
            return 0;
            break;
        case KEY_ENTER: 
            Display::delete_window(info_window);
            switch (hi_light) {
            case 0: 
                return 0; 
                break;
            case 1: 
                return 2; 
                break;
            case 2: 
                return 3; 
                break;
            case 3: 
                return 4; 
                break;
            }; 
            break;
        };
    }
}

// Print information in a separate window
bool Menu::print_info(int width, int height, char *string_with_info, bool select)
{
    int screen_width, screen_height;
    getmaxyx(stdscr, screen_height, screen_width);
    // Creating an information menu in the center of the screen
    info_window = newwin(height, width, (screen_height - height)/2, (screen_width - width)/2);
    Display::update(info_window);
    // and output the text
    Display::print_scr(info_window, width - 6, 0, (char*)"INFO", BLUE);
    Display::print_scr(info_window, 1, 1, string_with_info, BLUE);
    
    if (select) { // If this is a question (with a choice of answer in the form of "Yes" or " No")
        wattron(info_window, COLOR_PAIR(GREEN));
        std::string select_str[2] = {"<No> ", "<Yes>"};
        bool hi_light = false;
        while (1) {
            Display::print_scr(info_window, (width/2 - 2), height - 2,(char*)select_str[hi_light].c_str());
            switch (Periph::get_button()) {   // Processing keystrokes
            case KEY_LEFT: 
                hi_light = false; 
                break;
            case KEY_RIGHT: 
                hi_light = true; 
                break;
            case KEY_ENTER: 
                wattroff(info_window, COLOR_PAIR(GREEN)); 
                Display::delete_window(info_window); 
                return hi_light; // Return the answer
                break;
            };
        } 
    }
    return false;
}

// Return the map settings
CONFIG& Menu::get_config_map()
{ 
    return config_map; 
}   

// The return of the control settings
int *Menu::get_control()
{ 
    return buttons; 
}
