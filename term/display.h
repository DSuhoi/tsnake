/*** * * * * * * * * * * * * * * * * * * * * * ***
 * display.h                                     *
 * This library is needed to simplify interaction*
 * between the game components and the terminal  *
 * screen. The update and delete Window functions*
 * are often used to update, delete, and clear   *
 * Windows. To transfer the program to Windows   *
 * or Mac, you need to edit this file.            *
 *                                               *
 * Created by DSuhoi (2020) (C++11)              *
 *** * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>

// Enumerations for changing the color of a character
enum COLORS {
	GREEN = 1, 
	YELLOW, 
	RED, 
	BLUE 
};

// Display class
class Display {
private:
	Display(){}
public:
	static void Update(WINDOW *win);	// Update the window
	static void Update();				// Updating the entire screen
	static void DeleteWindow(WINDOW *win);	// Deleting the window
	
	static void InitColor();	// Initializing primary colors
	static void PrintScr(WINDOW *win, int x, int y, chtype ch);	// Print the character
	static void PrintScr(WINDOW *win, int x, int y, char *buff);	// Print the text
	static void PrintScr(WINDOW *win, int x, int y, char *buff, int color);	// Print the color text
	
};

#endif
