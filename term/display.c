#include <ncurses.h>
#include "display.h"


//инициализация основных цветов
void init_color(){	//присвоение цветов
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
}

//вывод текста
void printScr(WINDOW *win, int x, int y, char *buff){
	mvwprintw(win, y, x, "%s", buff);	//отсылаем массив по координатам
	wrefresh(win);	//и обновляем окно
}

//вывод цветного текста
void printScr(WINDOW *win, int x, int y, char *buff, int color){
	wattron(win,COLOR_PAIR(color));	//задаем цвет
	printScr(win, x, y, buff);
	wattroff(win,COLOR_PAIR(color));//отключаем цвет
	wrefresh(win);		//и обновляем окно
}

//вывод символа
void printScr(WINDOW *win, int x, int y, chtype ch){
	wmove(win,y,x);	//ставим курсор на позицию
	wdelch(win);	//удаляем символ
	winsch(win,ch);	//вставляем свой символ
	wrefresh(win);	//обновляем окно
}
