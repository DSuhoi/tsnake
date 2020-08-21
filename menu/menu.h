/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * menu.h                                                          *
 * The Menu class contains pointers to Windows, as well as         *
 * control settings and maps. The class contains methods that are  *
 * responsible for the settings and information  menu. As the game *
 * progresses, this file will be updated with new elements.         *
 *                                                                 *
 * Created by DSuhoi (2020) (C++11)                                *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/

#ifndef __MENU_H__
#define __MENU_H__


#include "../term/periph.h"
#include "../term/files.h"

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
class Menu {
private:
	static WINDOW *menuWidow;		// Pointer to the main menu
	static WINDOW *infoWidow;		// Pointer to the info menu
	static int buttons[4];			// Control settings
	static CONFIG configMap;			// <ap settings

	Menu(){}	// Constructor
	static void LvlSettingsLoop();		// Settings menu of the game
	static void ControlSettingsLoop();	// Settings menu of the control
public:
	static void InitMainMenu();	// Initializing the main menu
	static int MainMenuLoop(); 	// Main menu loop
	static void HelpLoop();		// Help loop
	static int PauseLoop();		// Pause loop
	static bool PrintInfo(int width, int height, char *stringWithInfo, bool select);	// Print information in a separate window
	static CONFIG& GetConfigMap();		// Return the map settings
	static int *GetControl();			// The return of the control settings
};

#endif
