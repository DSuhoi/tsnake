#include "display.h"


// Updating the entire screen
void Display::Update()
{
	refresh();
}

// Updating the window
void Display::Update(WINDOW *win)
{
	wclear(win);	// Очищаем окно
	wrefresh(win);	// Обновляем окно
	box(win, 0, 0);	// Устанавливаем границы окна
	refresh();	// Обновление
}


// Delete the window
void Display::DeleteWindow(WINDOW *win)
{
	if(win != nullptr){	// If the window pointer is not empty
		wclear(win);	// Clear the window
		wrefresh(win);	// Update the window
		delwin(win); 	// Delete the window
	} 
	win = nullptr; 
}

// Initializing primary colors
void Display::InitColor()
{	// Color assignment
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
}

// Print the text
void Display::PrintScr(WINDOW *win, int x, int y, char *buff)
{
	mvwprintw(win, y, x, "%s", buff);	// Sending the array by coordinates
	wrefresh(win);	// and updating the window
}

// Print the color text
void Display::PrintScr(WINDOW *win, int x, int y, char *buff, int color)
{
	wattron(win,COLOR_PAIR(color));	// Setting the color
	PrintScr(win, x, y, buff);
	wattroff(win,COLOR_PAIR(color));// Disable the color
	wrefresh(win);		// and updating the window
}

// Print the character
void Display::PrintScr(WINDOW *win, int x, int y, chtype ch)
{
	wmove(win,y,x);	// Putting the cursor on the position
	wdelch(win);	// Deleting the symbol
	winsch(win,ch);	// Insert your own character
	wrefresh(win);	// Updating the window
}
