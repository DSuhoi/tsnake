/*** * * * * * * * * * * * * * * * * * * ***
 * display.h                               *
 * Эта библиотека нужна для упрощения      *
 * взаимодействия между компонентами игры  *
 * и экраном терминала. Функции update и   *
 * deleteWindow часто используются         *
 * для обновления, удаления и очистки окон.*
 * Для переноса программы на Windows или   *
 * Mac нужно редактировать этот файл.      *
 * Created by DSuhoi (2020) (C file)       *
 *** * * * * * * * * * * * * * * * * * * ***/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>

//Константы для изменения цвета символа
enum COLORS {GREEN = 1, YELLOW, RED, BLUE };


inline void update(WINDOW *win){ wclear(win); wrefresh(win); box(win, 0, 0); refresh(); }	//обновление окна
inline void update(){ refresh(); }	//обновление экрана в целом
inline void deleteWindow(WINDOW *win){ if(win!=NULL){ wclear(win); wrefresh(win); delwin(win); } win = NULL; }	//удаление окна

void init_color();	//инициализация основных цветов
void printScr(WINDOW *win, int x, int y, chtype ch);	//вывод символа
void printScr(WINDOW *win, int x, int y, char *buff);	//вывод текста
void printScr(WINDOW *win, int x, int y, char *buff, int color);	//вывод цветного текста	


#endif
