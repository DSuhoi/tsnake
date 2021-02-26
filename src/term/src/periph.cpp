#include <ncurses.h>
#include <unistd.h> 
#include "menu.h"
#include "periph.h"


// Function-manual delay wrapper
void Periph::gameDelay(int delayTime)
{ 
    usleep(delayTime * SLEEP_TIME); 
}

// Initializing the screen and key processing
void Periph::initPeriph()
{
    initscr();
    start_color();
    noecho();
    nodelay(stdscr,true);
    keypad(stdscr,true);
    curs_set(0);
}

// End of key processing functions
void Periph::erasePeriph()
{
    keypad(stdscr,false);
    cbreak();
    endwin();
}

// Button processing
int Periph::getButton()
{
    
    usleep(100000); // Delay for recieve
    int button = getch();
    flushinp();     // Clearing the keyboard buffer
    int selectButton = ERR;
    switch (button) {
    case KEY_DOWN: 
        selectButton = KEY_DOWN; 
        break;
    case KEY_UP: 
        selectButton = KEY_UP; 
        break;
    case KEY_LEFT: 
        selectButton = KEY_LEFT; 
        break;
    case KEY_RIGHT: 
        selectButton = KEY_RIGHT; 
        break;
    case 10: 
        selectButton = KEY_ENTER; 
        break;
    case 27: 
    case 'q': 
    case 'Q': 
        selectButton = KEY_EXIT; 
        break;
    case 'h': 
    case 'H': 
        selectButton = 'h'; 
        break;
    case 'p': 
    case 'P': 
        selectButton = 'p'; 
        break;
    default: 
        selectButton = ERR; 
        break;
    };
    return selectButton;
}

// Processing of game buttons
int Periph::getButton(int *gameButtons, float pauseDelay)
{
    usleep(pauseDelay*30000);
    int button = getch();
    flushinp();
    int selectButton = ERR;
    if (button == gameButtons[DOWN]) { 
        selectButton = KEY_DOWN;
    } else if (button == gameButtons[UP]) { 
        selectButton = KEY_UP;
    } else if (button == gameButtons[LEFT]) { 
        selectButton = KEY_LEFT;
    } else if (button == gameButtons[RIGHT]) { 
        selectButton = KEY_RIGHT;
    } else { 
        switch (button) {
        case 10: 
            selectButton = KEY_ENTER; 
            break;
        case 27: 
        case 'q': 
        case 'Q': 
            selectButton = KEY_EXIT; 
            break;
        case 'h': 
        case 'H': 
            selectButton = 'h'; 
            break;
        case 'p': 
        case 'P': 
            selectButton = 'p'; 
            break;
        default: 
            selectButton = ERR; 
            break;
        };
    }   
    return selectButton;
}
