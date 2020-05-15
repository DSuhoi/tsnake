#include <iostream>
#include <ncurses.h>
#include "../menu/menu.h"
#include "periph.h"
//#include <unistd.h> 


void initPeriph(){
	initscr();
	start_color();
	noecho();
	halfdelay(1);
	//nodelay(stdscr,true);
	keypad(stdscr,true);
}


void endPeriph() {
	keypad(stdscr,false);	
	getch();
	endwin();
	exit(1);
}


int periph(){
	int tmp;
	int c = getch();
	//usleep(150000);
	if(c == KEY_DOWN) tmp = KEY_DOWN;
	else if(c == KEY_UP) tmp = KEY_UP;
	else if(c == KEY_LEFT) tmp = KEY_LEFT;
	else if(c == KEY_RIGHT) tmp = KEY_RIGHT;
	else if(c == 10) tmp = KEY_ENTER;
	else if(c == 'q' || c == 'Q') tmp = KEY_EXIT;
	else if(c == 'h' || c == 'H') tmp = 'h';
	else if(c == 'p' || c == 'P') tmp = 'p';
	else tmp = ERR;
		
	return tmp;
}



int periph(CONTROL ctr){
	int tmp;
	int c = getch();
	//usleep(150000);
	if(c == ctr.down) tmp = KEY_DOWN;
	else if(c == ctr.up) tmp = KEY_UP;
	else if(c == ctr.left) tmp = KEY_LEFT;
	else if(c == ctr.right) tmp = KEY_RIGHT;
	else if(c == 10) tmp = KEY_ENTER;
	else if(c == 'q' || c == 'Q') tmp = KEY_EXIT;
	else if(c == 'h' || c == 'H') tmp = 'h';
	else if(c == 'p' || c == 'P') tmp = 'p';
	else tmp = ERR;
		
	return tmp;
}
