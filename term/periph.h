/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * periph.h                                                  *
 * Библиотека с функциями инициализации, отключения и работы *
 * обработчика клавиатуры.                                   *
 *                                                           *
 * Created by DSuhoi (2020) (C file)                         *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __PERIPH_H__
#define __PERIPH_H__

#include <ncurses.h>
#include <unistd.h>	//библиотека для usleep

const int SLEEP_TIME = 10000;	//минимальная задержка
const int CURS_KEY_MIN = 258;	//номер (скан-код) клавиши курсора "down"
const int CURS_KEY_MAX = 261;	//номер (скан-код) клавиши курсора "right"

inline void sleep(int tmp){ usleep(tmp*SLEEP_TIME); }	//функция ручной задержки

enum CONTROL {DOWN = 0, UP, LEFT, RIGHT};

inline int getButton(){ return getch(); }	//получение символа

void initPeriph();	//иницилизация экрана и обработки клавиш
void endPeriph();	//завершение функций обработки клавиш
int periph();		//обработка кнопок
int periph(int *ctr, float pause);	//обработка игровых кнопок


#endif
