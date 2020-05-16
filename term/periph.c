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
}


int periph(){
	int tmp;
	int c = getch();
	//usleep(150000);
	switch(c){
	case KEY_DOWN: tmp = KEY_DOWN; break;
	case KEY_UP: tmp = KEY_UP; break;
	case KEY_LEFT: tmp = KEY_LEFT; break;
	case KEY_RIGHT: tmp = KEY_RIGHT; break;
	case 10: tmp = KEY_ENTER; break;
	case 'q': case 'Q': tmp = KEY_EXIT; break;
	case 'h': case 'H': tmp = 'h'; break;
	case 'p': case 'P': tmp = 'p'; break;
	default: tmp = ERR; break;
	};
	return tmp;
}



int periph(int *ctr){
	int tmp;
	int c = getch();
	//usleep(150000);
	if(c == ctr[DOWN]) tmp = KEY_DOWN;
	else if(c == ctr[UP]) tmp = KEY_UP;
	else if(c == ctr[LEFT]) tmp = KEY_LEFT;
	else if(c == ctr[RIGHT]) tmp = KEY_RIGHT;
	else if(c == 10) tmp = KEY_ENTER;
	else if(c == 'q' || c == 'Q') tmp = KEY_EXIT;
	else if(c == 'h' || c == 'H') tmp = 'h';
	else if(c == 'p' || c == 'P') tmp = 'p';
	else tmp = ERR;
		
	return tmp;
}
