#include <ncurses.h>
#include "display.h"


//перегрузка операторов сравнения для координат
bool operator ==(Coords &cd1, Coords &cd2){
	return (cd1.x == cd2.x && cd1.y == cd2.y) ? true : false;
}

bool operator !=(Coords &cd1, Coords &cd2){
	return (cd1 == cd2) ? false : true;
}

//инициализация основных цветов
void Display::init_color(){	//присвоение цветов
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
}

//вывод текста
void Display::printScr(WINDOW *win, int x, int y, char *buff){
	mvwprintw(win, y, x, "%s", buff);	//отсылаем массив по координатам
	wrefresh(win);	//и обновляем окно
}

//вывод цветного текста
void Display::printScr(WINDOW *win, int x, int y, char *buff, int color){
	wattron(win,COLOR_PAIR(color));	//задаем цвет
	printScr(win, x, y, buff);
	wattroff(win,COLOR_PAIR(color));//отключаем цвет
	wrefresh(win);		//и обновляем окно
}

//вывод символа
void Display::printScr(WINDOW *win, int x, int y, chtype ch){
	wmove(win,y,x);	//ставим курсор на позицию
	wdelch(win);	//удаляем символ
	winsch(win,ch);	//вставляем свой символ
	wrefresh(win);	//обновляем окно
}
