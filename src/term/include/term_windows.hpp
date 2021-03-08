#ifndef __TERM_WINDOWS_H__
#define __TERM_WINDOWS_H__

#include <memory>
#include <string>
#include <ncurses.h>

// Class the main windows
class Term_window
{
public:
    // Default constructor
    Term_window() = default;
    // Delete copy constructor
    Term_window(Term_window const &) = delete;
    void operator=(Term_window const &) = delete;
    // Basic constructor
    Term_window(int height, int width, int pos_y, int pos_x);
    // Constructor with color
    Term_window(int height, int width, int pos_y, int pos_x, chtype colors);
    // Virtual destructor
    virtual ~TERM_WINDOW();
    // Function for adding a color scheme
    void set_colors(chtype colors);
    // This function is an analog refresh() for two windows (main and background)
    virtual void update();
    // Clear the text in the main subwindow (wclear)
    virtual void clear();
    // Print the symbol in the main subwindow
    virtual void print(int x, int y, chtype ch);
    // Print the text in the main subwindow (mvwprintw)
    virtual void print(int x, int y, std::string const &text);
    // Get the main subwindow width and height
    void get_width_and_height(int &width, int &height);
private:
    WINDOW *main;       // This is the main subwindow
    WINDOW *background; // and its background window
};

#endif
