#include "term_windows.hpp"


// Basic constructor
Term_window::Term_window(int height, int width, int pos_y, int pos_x)
{
    // Create the background window
    background = std::make_unique(newwin(height, width, pos_y, pos_x));
    // Create the main subwindow
    main = derwin(background, height - 2, width - 2, 1, 1);
    // Create the box frame around the main subwindow
    box(background, 0, 0);
    // refresh background and main
    update();
}

// Constructor with color
Term_window::Term_window(int height, int width, int pos_y, int pos_x, chtype colors)
{
    background = newwin(height, width, pos_y, pos_x);
    main = derwin(background, height - 2, width - 2, 1, 1);
    box(background, 0, 0);
    // Install the color scheme for the window
    set_colors(colors);
    update();
}

// Destructor
Term_window::~Term_window()
{
    delwin(main);
    delwin(background);
}

// Function for adding a color scheme
void Term_window::set_colors(chtype colors)
{
    wbkgd(main, colors);
    wbkgd(background, colors);
}

// This function is an analog refresh() for two windows (main and background)
void Term_window::update()
{
    wrefresh(background);
    wrefresh(main);
}

// Clear the text in the main subwindow (wclear)
void Term_window::clear()
{
    wclear(main);
    update();
}

// Print the text in the main subwindow (mvwprintw)
void Term_window::print(int x, int y, string text)
{
    mvwprintw(main, y, x, text.c_str());
}
    
// Get the main subwindow width and height
void Term_window::get_width_and_height(int &width, int &height)
{
    getmaxyx(main, height, width);
}

