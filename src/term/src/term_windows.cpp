#include "term_windows.hpp"

// Default constructor
Term_zone::Term_zone() : _zone(nullptr) {}

Term_zone::Term_zone(Term_zone &&zone) : _zone(zone._zone)
{
    zone._zone = nullptr;
}

Term_zone &Term_zone::operator=(Term_zone &&zone)
{
    delete _zone;
    _zone = zone._zone;
    zone._zone = nullptr;
    return *this;
}

// Basic constructor
Term_zone::Term_zone(int width, int height, int pos_x, int pos_y)
{
   _zone = newwin(height, width, pos_y, pos_x);
   update();
}

// Constructor with color
Term_zone::Term_zone(int width, int height, int pos_x, int pos_y, chtype colors)
{
   _zone = newwin(height, width, pos_y, pos_x);
   set_colors(colors);
   update();
}

// Subwindow constructor
Term_zone::Term_zone(Term_zone const &zone, int width, int height, int pos_x, int pos_y)
{
    _zone = derwin(zone._zone, height, width, pos_y, pos_x);
    update();
}

// Destructor
Term_zone::~Term_zone()
{
    delwin(_zone);
}


// Function for adding a color scheme
void Term_zone::set_colors(chtype colors) const
{
    wbkgd(_zone, colors);
}

// This function is an analog refresh() for zone
void Term_zone::update() const
{
    wrefresh(_zone);
}

// Clear the text in the zone
void Term_zone::clear() const
{
    wclear(_zone);
    update();
}

// Print the symbol in the main subwindow
void Term_zone::print(int y, int x, chtype ch) const
{
    wmove(_zone, y, x); // Putting the cursor on the position
    wdelch(_zone);      // Deleting the symbol
    winsch(_zone, ch);  // Insert your own character
}

// Print the text in the main subwindow (mvwprintw)
void Term_zone::print(int y, int x, std::string_view text) const
{
    mvwprintw(_zone, y, x, static_cast<std::string>(text).c_str());
}

// Print the text in the main subwindow (mvwprintw)
void Term_zone::print(int y, int x, std::string_view text, int color) const
{
    wattron(_zone, COLOR_PAIR(color));   // Setting the color
    print(y, x, text);
    wattroff(_zone, COLOR_PAIR(color));
}

// This function is an analog box() for zone
void Term_zone::set_box(chtype border_chr) const
{
    box(_zone, border_chr, border_chr);
}

// Get the main subwindow width
int Term_zone::get_width() const
{
    int width, height;
    getmaxyx(_zone, height, width);
    return width;
}

// Get the main subwindow height
int Term_zone::get_height() const
{
    int width, height;
    getmaxyx(_zone, height, width);
    return height;
}

//////////////////////////////////////////////////////////////////////////////////////////

// Basic constructor
Term_window::Term_window(int width, int height, int pos_x, int pos_y, chtype border_chr)
{
    // Create the background window
    _background = Term_zone(height, width, pos_y, pos_x);
    // Create the main subwindow
    _main = Term_zone(_background, height - 2, width - 2, 1, 1);
    // Create the box frame around the main subwindow
    _background.set_box(border_chr);
    // refresh background and main
    update();
}

// Constructor with color
Term_window::Term_window(int width, int height, int pos_x, int pos_y, chtype colors, chtype border_chr)
{
    _background = Term_zone(height, width, pos_y, pos_x);
    _main = Term_zone(_background, height - 2, width - 2, 1, 1);
    _background.set_box(border_chr);
    // Install the color scheme for the window
    set_colors(colors);
    update();
}

// Subwindow constructor
Term_window::Term_window(const Term_window &win, int width, int height, int pos_x, int pos_y, chtype border_chr)
{
    _background = Term_zone(win._main, height, width, pos_y, pos_x);
    _main = Term_zone(_background, height - 2, width - 2, 1, 1);
    _background.set_box(border_chr);
    update();
}

// Destructor
Term_window::~Term_window() 
{}

// Function for adding a color scheme
void Term_window::set_colors(chtype colors) const
{
    _background.set_colors(colors);
    _main.set_colors(colors);
}

// This function is an analog refresh() for two windows (main and background)
void Term_window::update() const
{
    _background.update();
    _main.update();
}

// Clear the text in the main subwindow (wclear)
void Term_window::clear() const
{
    _main.clear();
    update();
}

// Print the symbol in the main subwindow
void Term_window::print(int y, int x, chtype chr) const
{
    _main.print(y, x, chr);
}

// Print the text in the main subwindow
void Term_window::print(int y, int x, std::string_view text) const
{
    _main.print(y, x, text);    
}

// Print the text in the main subwindow (mvwprintw)
void Term_window::print(int y, int x, std::string_view text, int color) const
{
    _main.print(y, x, text, color);    
}

// Print the label in the window
void Term_window::print_label(int x, std::string_view text, int color) const
{
    _background.print(0, x, text, color);
}
   
// Get the main subwindow width
int Term_window::get_width() const
{
    return _main.get_width();
}

// Get the main subwindow height
int Term_window::get_height() const
{
    return _main.get_height();
}
