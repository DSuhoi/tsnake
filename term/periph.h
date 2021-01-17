/*** * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * periph.h                                                *
 * The library with functions for initializing, disabling, *
 * and running the keyboard handler.                       *
 *                                                         *
 * Created by DSuhoi (2020) (C++11)                        *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __PERIPH_H__
#define __PERIPH_H__

#include <ncurses.h>
#include <unistd.h>     // Library for usleep() function

const int SLEEP_TIME = 10000;   // Minimum delay
const int CURS_KEY_MIN = 258;   // Number (scan code) of the "down" cursor key"
const int CURS_KEY_MAX = 261;   // Number (scan code) of the "right" cursor key"

enum CONTROL {
	DOWN = 0, 
	UP, 
	LEFT, 
	RIGHT
};

// Peripherals class
class Periph{
private:
	Periph(){}
public:
	static void gameDelay(int delayTime);   // Function-manual delay wrapper
	static void initPeriph();   // Initializing the screen and key processing
	static void erasePeriph();  // End of key processing functions
	
	static int getButton();     // Button processing
	static int getButton(int *gameButtons, float pauseDelay);   // Processing of game buttons
};



#endif
