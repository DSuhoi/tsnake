/*** * * * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * periph.h                                                  *
 * Библиотека с функциями инициализации, отключения и работы *
 * обработчика клавиатуры.                                   *
 *                                                           *
 * Created by DSuhoi (2020) (C++11)                          *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __PERIPH_H__
#define __PERIPH_H__

#include <ncurses.h>
#include <unistd.h>		// Библиотека для usleep

const int SLEEP_TIME = 10000;	// Минимальная задержка
const int CURS_KEY_MIN = 258;	// Номер (скан-код) клавиши курсора "down"
const int CURS_KEY_MAX = 261;	// Номер (скан-код) клавиши курсора "right"

enum CONTROL {
	DOWN = 0, 
	UP, 
	LEFT, 
	RIGHT
};


class Periph{
private:
	Periph(){}
public:
	static void GameDelay(int delayTime);	// Метод-обёртка ручной задержки
	static void InitPeriph();	// Иницилизация экрана и обработки клавиш
	static void ErasePeriph();	// Завершение функций обработки клавиш
	
	static int GetButton();		// Обработка кнопок
	static int GetButton(int *gameButtons, float pauseDelay);	// Обработка игровых кнопок
};



#endif
