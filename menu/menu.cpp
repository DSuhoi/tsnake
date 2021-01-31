#include <iostream>
#include <fstream>
#include <cstring>
#include "../term/display.h"
#include "menu.h"

// Defining Menu class fields
WINDOW *Menu::menuWidow;
WINDOW *Menu::infoWidow;
int Menu::buttons[4];
CONFIG Menu::configMap;


// Initializing the main menu
void Menu::initMainMenu()
{
    int screenWidth = 0, screenHeight = 0;  // Screen size X*Y
    getmaxyx(stdscr, screenHeight, screenWidth);
    // Creating the main menu window
    menuWidow = newwin(MENU_HEIGHT, MENU_WIDTH,(screenHeight - MENU_HEIGHT)/2, (screenWidth - MENU_WIDTH)/2);
    // Control and map settings
    buttons[DOWN] = KEY_DOWN;
    buttons[UP] = KEY_UP;
    buttons[LEFT] = KEY_LEFT;
    buttons[RIGHT] = KEY_RIGHT;
    
    configMap = FileSystem::loadSettings(buttons);
}

// Main menu loop
int Menu::mainMenuLoop()
{
    // Array of menu item names
    std::string menuPart[5] ={"Start Game", "Lvl Settings", "Controls", "Help", "Exit"};
    
    // Clearing and updating the window
    Display::update(menuWidow);
    // Selecting a menu elements (highlighting the selected item)
    unsigned int hiLight = 0;    
    while (1) {
        // Output the name of the game
        Display::printScr(menuWidow, MENU_WIDTH/2 - 5, 0, (char*)"TSNAKE 1.3", BLUE);
    
        // Green elements
        for (unsigned int cursPosition = 0; cursPosition < 5; ++cursPosition) {
            if (cursPosition == hiLight)   // Highlight the selected line
                wattron(menuWidow, COLOR_PAIR(GREEN));
            // Print lines
            Display::printScr(menuWidow, 2, cursPosition + 1, (char*)menuPart[cursPosition].c_str());
            wattroff(menuWidow, COLOR_PAIR(GREEN));
        }
        // Reading the keystroke
        switch (Periph::getButton()) {
        case KEY_UP: 
            if (hiLight > 0)     // Selecting a menu item
                --hiLight;
            break;  
        case KEY_DOWN: 
            if (hiLight < 4)
                ++hiLight; 
            break;
        case KEY_EXIT: 
            return 0;            // Exit the program
            break;      
        case KEY_ENTER:
            switch (hiLight) {
            case 0: 
                return 1;        // If this is a game, then exit the menu
                break;  
            case 1: 
                lvlSettingsLoop();      // Level settings
                break;      
            case 2: 
                controlSettingsLoop();  // Control settings
                break;  
            case 3: 
                helpLoop(); // Information
                break;
            case 4: 
                return 0;   // Exit the program
                break;
            }; 
            Display::update(menuWidow);
            break;
        };
    }
}

// Settings menu of the game
void Menu::lvlSettingsLoop()
{
    // Menu elements, map parameters, and responses
    std::string menuPart[8] = {"Back", "Speed:", "Fruit:","Map Size:", 
        "Border:", "Teleport:", "Erase Settings", "Erase Records"};
    std::string mapSize[3] = {"Small", "Medium", "Large"};
    std::string selectStr[2] = {"Off", "On"};
    // Character array for displaying menu lines
    char buffStr[10];
    // Updating the menu window 
    Display::update(menuWidow);
    // Type of brackets (0 - rectangular, 1-sharp)
    int currentBracket = 0;
    unsigned int hiLight = 0;
    while (1) {
    
        Display::printScr(menuWidow, MENU_WIDTH/2 - 6, 0, (char*)"Lvl Settings", BLUE);
        
        for (unsigned int cursPosition = 0; cursPosition < 8; ++cursPosition) {
            if (cursPosition == hiLight) {
                // Enable the green light for the selected element
                wattron(menuWidow, COLOR_PAIR(GREEN));
                currentBracket = 1;
            } else { 
                currentBracket = 0;
            }
            
            // Defining a parameter based on the position of the menu rendering cursor
            char parametr[7] = "";
            switch (cursPosition) {
            case 1:
                sprintf(parametr, "%d", configMap.speed);
                break;
            case 2:
                sprintf(parametr, "%d", configMap.numFruits);
                break;
            case 3:
                strcpy(parametr, (char*)mapSize[configMap.mapSize].c_str());
                break;
            case 4:
                strcpy(parametr, (char*)selectStr[configMap.border].c_str());
                break;
            case 5:
                strcpy(parametr, (char*)selectStr[configMap.teleport].c_str());
                break;
            };
            
            if (cursPosition == 0) {
                // Print the exit parameter from the menu
                Display::printScr(menuWidow, 2, 1, (char*)menuPart[0].c_str());
            } else if (cursPosition > 5) {
                // Print the parameter for clearing settings
                Display::printScr(menuWidow, 2, (MENU_HEIGHT - 9) + cursPosition, (char*)menuPart[cursPosition].c_str());
            } else {
                // Left and right brackets
                char bracketL[2] = {'[', '<'};
                char bracketR[2] = {']', '>'};
                // Generating the menu bar
                sprintf(buffStr,"%c%s%c", bracketL[currentBracket], parametr, bracketR[currentBracket]);
                // Displays the parameter name and its value (centered)
                Display::printScr(menuWidow, MENU_WIDTH - strlen(buffStr)/2 - 5, cursPosition + 2, buffStr);
                Display::printScr(menuWidow, 2, cursPosition + 2, (char*)menuPart[cursPosition].c_str());
            }
        
            // Clearing the array
            for(unsigned int index = 0; index < 9; index++)
                buffStr[index] = 0;
            // Disable the green light
            wattroff(menuWidow, COLOR_PAIR(GREEN));
        }
        // Processing keystrokes
        switch(Periph::getButton()) {
        case KEY_UP: 
            if (hiLight > 0)
                --hiLight; 
            break;
        case KEY_DOWN: 
            if (hiLight < 7)
                ++hiLight;
            break;
        case KEY_LEFT: // Depending on the element, configure the parameters
            switch(hiLight) {
            case 1: 
                // Speed reduction, number of fruits on the map and its size
                if(configMap.speed > 1)
                    --configMap.speed;
                break;
            case 2: 
                if(configMap.numFruits > 1)
                    --configMap.numFruits;
                break;
            case 3: 
                if(configMap.mapSize > 0)
                    --configMap.mapSize;
                break;
            case 4: 
                // Disabling obstacles
                configMap.border = false; 
                break;
            case 5: 
                // Disables the ability to teleport
                configMap.teleport = false; 
                break;
            };
            // Updating the settings menu
            Display::update(menuWidow);
            break;
        case KEY_RIGHT:
            switch(hiLight) {
            case 1: 
                if(configMap.speed < 10)
                    ++configMap.speed; 
                break;
            case 2: 
                if(configMap.numFruits < 99)
                    ++configMap.numFruits;
                break;
            case 3: 
                if(configMap.mapSize < 2)
                    ++configMap.mapSize;
                break;
            case 4: 
                configMap.border = true; 
                break;
            case 5: 
                configMap.teleport = true; 
                break;
            };
            // Updating the settings menu
            Display::update(menuWidow);
            break;
        case KEY_EXIT: 
            FileSystem::saveSettings(configMap,buttons); 
            return; 
            break;
        case KEY_ENTER: 
            if (hiLight == 0) { 
                // Saving settings in a file when exiting the menu
                FileSystem::saveSettings(configMap,buttons); 
                return; 
            } else if (hiLight == 6) {
                // Default settings
                configMap = {8, 1, 1, false, false, false};
                // Updating the settings menu
                Display::update(menuWidow); 
            } else if (hiLight == 7) {
                // Displays the menu about clearing records
                if (printInfo(INFO_WIDTH - 6, INFO_HEIGHT,(char*)"CLEAR DATA ?", true))
                    configMap.clearScore = true;
            }
            // Update the screen
            Display::update();
            break;
        };
    }
}

// Settings menu of the control
void Menu::controlSettingsLoop()
{
    // Menu elements and cursor key names (could be done via map container)
    std::string menuPart[7] = {"Back", "Key Down:", "Key Up:", "Key Left:", "Key Right:", "Erase Settings"};
    std::string cursKey[4] = {"down ", "up  ", "left ", "right"};
    
    Display::update(menuWidow);
    
    unsigned int hiLight = 0;
    while (1) {
    
        Display::printScr(menuWidow, MENU_WIDTH/2 - 4, 0, (char*)"Controls", BLUE);
    
        for (unsigned int cursPosition = 0; cursPosition < 6; ++cursPosition) {
            // Cursor highlighting
            if (cursPosition == hiLight)
                wattron(menuWidow, COLOR_PAIR(GREEN));
            // Button display
            if (0 < cursPosition && cursPosition < 5){
                if (buttons[cursPosition - 1] < CURS_KEY_MIN || CURS_KEY_MAX < buttons[cursPosition - 1])
                    Display::printScr(menuWidow, MENU_WIDTH - 7, cursPosition + 2, buttons[cursPosition - 1]);
                else
                    Display::printScr(menuWidow, MENU_WIDTH - 7, cursPosition + 2, (char*)cursKey[buttons[cursPosition - 1] - CURS_KEY_MIN].c_str()); 
            }
        
            if (cursPosition == 0)
                // Print the exit parameter from the menu
                Display::printScr(menuWidow, 2, 1, (char*)menuPart[0].c_str());
            else if (cursPosition == 5)
                // Print the parameter for clearing settings
                Display::printScr(menuWidow, 2, MENU_HEIGHT - 3, (char*)menuPart[5].c_str());
            else
                Display::printScr(menuWidow, 2, cursPosition + 2, (char*)menuPart[cursPosition].c_str());
        
            wattroff(menuWidow, COLOR_PAIR(GREEN));
        }
    
        switch (Periph::getButton()) {
        case KEY_UP: 
            if (hiLight > 0)
                --hiLight;
            break;
        case KEY_DOWN: 
            if (hiLight < 5)
                ++hiLight; 
            break;
        case KEY_EXIT:
            // Saving keys in a file when exiting the menu
            FileSystem::saveSettings(configMap,buttons); 
            return; 
            break;
        case KEY_ENTER: 
            if (hiLight == 0) {
                // Saving keys in a file when exiting the menu
                FileSystem::saveSettings(configMap,buttons); 
                return; 
            } else if (hiLight == 5) {
                // Reset keys
                buttons[DOWN] = KEY_DOWN;
                buttons[UP] = KEY_UP;
                buttons[LEFT] = KEY_LEFT;
                buttons[RIGHT] = KEY_RIGHT;
            } else {
                // Creating an information window for processing the pressed key
                printInfo(INFO_WIDTH, INFO_HEIGHT - 1, (char*)"Press the button!", false);
                cbreak();
                nodelay(stdscr, false);         // Disable the delay
                int recievedButton = getch();   // Recieve the symbol
                nodelay(stdscr, true);          // Enable the delay
                Display::deleteWindow(infoWidow);   // Delete the window
                // Assign a character
                switch (hiLight) {
                case 1: 
                    buttons[DOWN] = recievedButton; 
                    break;
                case 2: 
                    buttons[UP] = recievedButton; 
                    break;
                case 3: 
                    buttons[LEFT] = recievedButton; 
                    break;
                case 4: 
                    buttons[RIGHT] = recievedButton; 
                    break;
                };
        
            }
            Display::update(menuWidow); 
            break;
        };
    
    }
}

// Information
void Menu::helpLoop()
{
    // Name of cursor keys
    std::string cursKey[4] = {"down", "up", "left", "right"};
    
    int screenWidth = 0, screenHeight = 0;
    getmaxyx(stdscr, screenHeight, screenWidth);
    // Creating the info window in the center of the screen
    infoWidow = newwin(HELP_HEIGHT, HELP_WIDTH, (screenHeight - HELP_HEIGHT)/2, (screenWidth - HELP_WIDTH)/2);
    Display::update(infoWidow);
    
    bool select = false;
    while (1) {
        // Print information about control and the game
        Display::printScr(infoWidow, HELP_WIDTH - 6, 0, (char*)"HELP", BLUE);
    
        if (!select) {
            Display::printScr(infoWidow, 1, 0, (char*)"(1)", BLUE);
            Display::printScr(infoWidow, 2, 1, (char*)"Controls:", YELLOW);
            wattron(infoWidow, COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, 2, 2, (char*)"Key Down:");
            Display::printScr(infoWidow, 2, 3, (char*)"Key Up:");
            Display::printScr(infoWidow, 2, 4, (char*)"Key Left:");
            Display::printScr(infoWidow, 2, 5, (char*)"Key Right:");
            Display::printScr(infoWidow, 2, 6, (char*)"Key Pause Game:");
            Display::printScr(infoWidow, 2, 7, (char*)"Key Help Game:");
            Display::printScr(infoWidow, 2, 8, (char*)"Key Quit Game:");
            wattroff(infoWidow, COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, Help_WIDTH_RIGHT, 6, 'p');
            Display::printScr(infoWidow, Help_WIDTH_RIGHT, 7, 'h');
            Display::printScr(infoWidow, Help_WIDTH_RIGHT, 8, 'q');
    
            // Print of control keys
            for (int currentButton = 0; currentButton < 4; currentButton++)
                if (buttons[currentButton] < CURS_KEY_MIN || CURS_KEY_MAX < buttons[currentButton])
                    Display::printScr(infoWidow, Help_WIDTH_RIGHT, currentButton + 2, buttons[currentButton]);
                else
                    Display::printScr(infoWidow, Help_WIDTH_RIGHT, currentButton + 2, (char*)cursKey[buttons[currentButton]-CURS_KEY_MIN].c_str());
        } else {
            // Print reference information
            Display::printScr(infoWidow, 1, 0, (char*)"(2)", BLUE);
            Display::printScr(infoWidow, 2, 1, (char*)"About:", YELLOW);
            Display::printScr(infoWidow, 2, 2, (char*)"Game", GREEN);
            Display::printScr(infoWidow, 7, 2, 'T' | COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, 8, 2, 'S' | COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, 9, 2, 'N' | COLOR_PAIR(YELLOW));
            Display::printScr(infoWidow, 10, 2, 'A' | COLOR_PAIR(RED));
            Display::printScr(infoWidow, 11, 2, 'K' | COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, 12, 2, 'E'| COLOR_PAIR(BLUE));
            Display::printScr(infoWidow, 14, 2, (char*)"v_1.3", BLUE);
            wattron(infoWidow, COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, 2, 4, (char*)"Game created by DSuhoi (2020)");
            Display::printScr(infoWidow, 2, 5, (char*)"Email: <dsuh0i.h8@gmail.com>");
            Display::printScr(infoWidow, 2, 6, (char*)"Sourse Code:");
            Display::printScr(infoWidow, 2, 7, (char*)"https://github.com/DSuhoi/tsnake");
            Display::printScr(infoWidow, 5, 8, (char*)"Thanks for playing!");
            wattroff(infoWidow, COLOR_PAIR(GREEN));
        }
        switch (Periph::getButton()) {    //Exit the information menu
        case KEY_ENTER:
        case KEY_EXIT:
        case 'h':
            // Deleting the info window
            Display::deleteWindow(infoWidow); 
            return; 
            break;
        case KEY_LEFT: 
            select = false; 
            Display::update(infoWidow); 
            break;
        case KEY_RIGHT: 
            select = true; 
            Display::update(infoWidow); 
            break;
        };
    }
}

// Pause loop
int Menu::pauseLoop()
{
    // Window element
    std::string pauseMenuStr[4] = {"Resume", "Restart", "Quit to Menu", "Quit Game"};
    
    int screenWidth = 0, screenHeight = 0;
    getmaxyx(stdscr, screenHeight, screenWidth);
    // Create the window
    infoWidow = newwin(PAUSE_HEIGHT, PAUSE_WIDTH, (screenHeight - PAUSE_HEIGHT)/2, (screenWidth - PAUSE_WIDTH)/2);
    Display::update(infoWidow);
    Display::printScr(infoWidow, PAUSE_WIDTH - 7, 0, (char*)"PAUSE", BLUE);
    // Menu highlighting
    unsigned int hiLight = 0;
    while (1) {

        for (unsigned int i = 0; i < 4; ++i) {
            if (i == hiLight)
                wattron(infoWidow, COLOR_PAIR(GREEN));
            Display::printScr(infoWidow, 2, i + 1, (char*)pauseMenuStr[i].c_str());
            wattroff(infoWidow, COLOR_PAIR(GREEN));
        }

        switch (Periph::getButton()) {
        case KEY_UP:
            if (hiLight > 0)
                --hiLight;
            break;
        case KEY_DOWN:
            if (hiLight < 3)
                ++hiLight;
            break;
        case KEY_EXIT:
        case 'p':
            Display::deleteWindow(infoWidow);
            return 0;
            break;
        case KEY_ENTER: 
            Display::deleteWindow(infoWidow);
            switch (hiLight) {
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
bool Menu::printInfo(int width, int height, char *stringWithInfo, bool select)
{
    int screenWidth, screenHeight;
    getmaxyx(stdscr, screenHeight, screenWidth);
    // Creating an information menu in the center of the screen
    infoWidow = newwin(height, width, (screenHeight - height)/2, (screenWidth - width)/2);
    Display::update(infoWidow);
    // and output the text
    Display::printScr(infoWidow, width - 6, 0, (char*)"INFO", BLUE);
    Display::printScr(infoWidow, 1, 1, stringWithInfo, BLUE);
    
    if (select) { // If this is a question (with a choice of answer in the form of "Yes" or " No")
        wattron(infoWidow, COLOR_PAIR(GREEN));
        std::string selectStr[2] = {"<No> ", "<Yes>"};
        bool hiLight = false;
        while (1) {
            Display::printScr(infoWidow, (width/2 - 2), height - 2,(char*)selectStr[hiLight].c_str());
            switch (Periph::getButton()) {   // Processing keystrokes
            case KEY_LEFT: 
                hiLight = false; 
                break;
            case KEY_RIGHT: 
                hiLight = true; 
                break;
            case KEY_ENTER: 
                wattroff(infoWidow, COLOR_PAIR(GREEN)); 
                Display::deleteWindow(infoWidow); 
                return hiLight; // Return the answer
                break;
            };
        } 
    }
    return false;
}

// Return the map settings
CONFIG& Menu::getConfigMap()
{ 
    return configMap; 
}   

// The return of the control settings
int *Menu::getControl()
{ 
    return buttons; 
}
