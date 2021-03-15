#include "periph.hpp"
#include <thread>


// Function-manual delay wrapper
void Periph::game_delay(int delay_time)
{ 
    std::this_thread::sleep_for(delay_time * SLEEP_TIME); 
}

// Initializing the screen and key processing
void Periph::init_periph()
{
    initscr();
    start_color();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);
}

// End of key processing functions
void Periph::erase_periph()
{
    keypad(stdscr, false);
    cbreak();
    endwin();
}

// Button processing
int Periph::get_button()
{
    
    std::this_thread::sleep_for(10 * SLEEP_TIME); // Delay for recieve
    int button = getch();
    flushinp();     // Clearing the keyboard buffer
    int select_button = ERR;
    switch (button) {
    case KEY_DOWN: 
        select_button = KEY_DOWN; 
        break;
    case KEY_UP: 
        select_button = KEY_UP; 
        break;
    case KEY_LEFT: 
        select_button = KEY_LEFT; 
        break;
    case KEY_RIGHT: 
        select_button = KEY_RIGHT; 
        break;
    case 10: 
        select_button = KEY_ENTER; 
        break;
    case 27: 
    case 'q': 
    case 'Q': 
        select_button = KEY_EXIT; 
        break;
    case 'h': 
    case 'H': 
        select_button = 'h'; 
        break;
    case 'p': 
    case 'P': 
        select_button = 'p'; 
        break;
    default: 
        select_button = ERR; 
        break;
    };
    return select_button;
}

// Processing of game buttons
int Periph::get_button(int *game_buttons, float pause_delay)
{
    std::this_thread::sleep_for(pause_delay * 3 * SLEEP_TIME);
    int button = getch();
    flushinp();
    int select_button = ERR;
    if (button == game_buttons[DOWN]) { 
        select_button = KEY_DOWN;
    } else if (button == game_buttons[UP]) { 
        select_button = KEY_UP;
    } else if (button == game_buttons[LEFT]) { 
        select_button = KEY_LEFT;
    } else if (button == game_buttons[RIGHT]) { 
        select_button = KEY_RIGHT;
    } else { 
        switch (button) {
        case 10: 
            select_button = KEY_ENTER; 
            break;
        case 27: 
        case 'q': 
        case 'Q': 
            select_button = KEY_EXIT; 
            break;
        case 'h': 
        case 'H': 
            select_button = 'h'; 
            break;
        case 'p': 
        case 'P': 
            select_button = 'p'; 
            break;
        default: 
            select_button = ERR; 
            break;
        };
    }   
    return select_button;
}
