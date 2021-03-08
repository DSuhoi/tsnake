#ifndef __TERM_WINDOWS_H__
#define __TERM_WINDOWS_H__

#include <memory>
#include <string>
#include <ncurses.h>


class Term_zone
{
public:
    // Default constructor
    Term_zone() = default;
    // Delete copy constructor
    Term_zone(Term_zone const &) = default;
    // Basic constructor
    Term_zone(int height, int width, int pos_y, int pos_x);
    // Constructor with color
    Term_zone(int height, int width, int pos_y, int pos_x, chtype colors);
    // Subwindow constructor
    Term_zone(Term_zone const &zone, int height, int width, int pos_y, int pos_x);
    // Destructor
    ~Term_zone();

    // Function for adding a color scheme
    void set_colors(chtype colors) const;
    // This function is an analog refresh() for zone
    virtual void update() const;
    // Clear the text in the zone
    virtual void clear();
    // Print the symbol in the main subwindow
    virtual void print(int x, int y, chtype ch) const;
    // Print the text in the main subwindow (mvwprintw)
    virtual void print(int x, int y, std::string const &text) const;
    // This function is an analog box() for zone
    void set_box(chtype border_chr) const;
    // Get the main subwindow width
    int get_width() const;
    // Get the main subwindow height
    int get_height() const;
private:
    WINDOW *_zone;
};

// Class the main windows
class Term_window
{
public:
    // Default constructor
    Term_window() = default;
    // Delete copy constructor
    Term_window(Term_window const &) = default;
    // Basic constructor
    Term_window(int height, int width, int pos_y, int pos_x, chtype border_chr = 0);
    // Constructor with color
    Term_window(int height, int width, int pos_y, int pos_x, chtype colors, chtype border_chr = 0);
    // Subwindow constructor
    Term_window(const Term_window &win, int height, int width, int pos_y, int pos_x, chtype border_chr = 0);
    // Virtual destructor
    virtual ~Term_window();
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
    // Get the main subwindow width
    int get_width();
    // Get the main subwindow height
    int get_height();
private:
    Term_zone _main;       // This is the main subwindow
    Term_zone _background; // and its background window
};

#endif
