#include <ncurses.h>
#include "../menu/menu.h"
#include "periph.h"
#include <unistd.h> 


void initPeriph(){
	initscr();
	start_color();
	noecho();
	//halfdelay(1);
	nodelay(stdscr,true);
	keypad(stdscr,true);
}


void endPeriph() {
	keypad(stdscr,false);
	cbreak();
	endwin();
}


int periph(){
	int tmp;
	usleep(100000);	//задержка для приёма
	int c = getch();
	flushinp();	//очищаем буфер клавиатуры
	switch(c){
	case KEY_DOWN: tmp = KEY_DOWN; break;
	case KEY_UP: tmp = KEY_UP; break;
	case KEY_LEFT: tmp = KEY_LEFT; break;
	case KEY_RIGHT: tmp = KEY_RIGHT; break;
	case 10: tmp = KEY_ENTER; break;
	case 27: case 'q': case 'Q': tmp = KEY_EXIT; break;
	case 'h': case 'H': tmp = 'h'; break;
	case 'p': case 'P': tmp = 'p'; break;
	default: tmp = ERR; break;
	};
	return tmp;
}



int periph(int *ctr, float pause){
	int tmp;
	usleep(pause*30000);
	int c = getch();
	flushinp();
	if(c == ctr[DOWN]) tmp = KEY_DOWN;
	else if(c == ctr[UP]) tmp = KEY_UP;
	else if(c == ctr[LEFT]) tmp = KEY_LEFT;
	else if(c == ctr[RIGHT]) tmp = KEY_RIGHT;
	else switch(c){
	case 10: tmp = KEY_ENTER; break;
	case 27: case 'q': case 'Q': tmp = KEY_EXIT; break;
	case 'h': case 'H': tmp = 'h'; break;
	case 'p': case 'P': tmp = 'p'; break;
	default: tmp = ERR; break;
	};	
	return tmp;
}
