#ifndef __TERM_WINDOWS_H__
#define __TERM_WINDOWS_H__

#include <string>
#include <ncurses.h>


class Term_zone
{
public:
    // Default constructor
    Term_zone();
    // Delete copy constructor and operator
    Term_zone(const Term_zone &) = delete;
    Term_zone &operator=(Term_zone const &) = delete;
    // Move constructor and operator
    Term_zone(Term_zone &&zone);
    Term_zone &operator=(Term_zone &&zone);
    // Basic constructor
    Term_zone(int height, int width, int pos_y, int pos_x);
    // Constructor with color
    Term_zone(int height, int width, int pos_y, int pos_x, chtype colors);
    // Subwindow constructor
    Term_zone(const Term_zone &zone, int height, int width, int pos_y, int pos_x);
    // Destructor
    virtual ~Term_zone();

    // Function for adding a color scheme
    void set_colors(chtype colors) const;
    // This function is an analog refresh() for zone
    virtual void update() const;
    // Clear the text in the zone
    virtual void clear() const;
    // Print the symbol in the zone
    virtual void print(int y, int x, chtype ch) const;
    // Print the text in the zone
    virtual void print(int y, int x, std::string &text) const;
    // Print the label in the zone
    virtual void print(int y, int x, std::string &text, int color) const;
    // This function is an analog box() for zone
    void set_box(chtype border_chr) const;
    // Get the zone width
    int get_width() const;
    // Get the zone height
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
    Term_window(const Term_window &) = delete;
    Term_window &operator=(Term_window const &) = delete;
    // Basic constructor
    Term_window(int height, int width, int pos_y, int pos_x, chtype border_chr = 0);
    // Constructor with color
    Term_window(int height, int width, int pos_y, int pos_x, chtype colors, chtype border_chr = 0);
    // Subwindow constructor
    Term_window(const Term_window &win, int height, int width, int pos_y, int pos_x, chtype border_chr = 0);
    // Virtual destructor
    virtual ~Term_window();

    // Function for adding a color scheme
    void set_colors(chtype colors) const;
    // This function is an analog refresh() for two windows (main and background)
    virtual void update() const;
    // Clear the text in the main subwindow (wclear)
    virtual void clear() const;
    // Print the symbol in the main subwindow
    virtual void print(int y, int x, chtype chr) const;
    // Print the text in the main subwindow (mvwprintw)
    virtual void print(int y, int x, std::string &text) const;
    // Print the text in the main subwindow (mvwprintw)
    virtual void print(int y, int x, std::string &text, int color) const;
    // Print the label in the window
    void print_label(int x, std::string &text, int color) const;
    // Get the main subwindow width
    int get_width() const;
    // Get the main subwindow height
    int get_height() const;
private:
    Term_zone _main;
    Term_zone _background;
};

#endif
