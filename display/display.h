/*** * * * * * * * * * * * * * * * * * * ***
 * display.h                               *
 * Этот класс Display нужен для упрощения  *
 * взаимодействия между компонентами игры  *
 * и экраном терминала. Функции update, cls*
 * и deleteWindow не входят в него, т.к.   *
 * они часто используются для обновления,  *
 * удаления и очистки окон. Для переноса   *
 * программы на Windows или Mac нужно реда-*
 * ктировать именно этот класс и функции.  *
 * Created by DSuhoi (2020) (C++11)        *
 *** * * * * * * * * * * * * * * * * * * ***/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>

//Константы для изменения цвета символа
enum COLORS {GREEN = 1, YELLOW, RED, BLUE };

class Coords {	//класс координат
public:
	int x;
	int y;
		//перегрузка операторов сравнения для координат
	friend bool operator ==(Coords &cd1, Coords &cd2);
	friend bool operator !=(Coords &cd1, Coords &cd2);
};

inline void update(WINDOW *win) {box(win, 0, 0); refresh(); }	//обновление окна
inline void update(){ refresh(); }	//обновление экрана в целом
inline void cls(WINDOW *win) { wclear(win); wrefresh(win); } //очистка окна
inline void deleteWindow(WINDOW *win){ if(win!=NULL){ cls(win); delwin(win); } win = NULL; }	//удаление окна


class Display {
public:
	static void init_color();	//инициализация основных цветов
	static void printScr(WINDOW *win, int x, int y, chtype ch);	//вывод символа
	static void printScr(WINDOW *win, int x, int y, char *buff);	//вывод текста
	static void printScr(WINDOW *win, int x, int y, char *buff, int color);	//вывод цветного текста	
};

#endif
