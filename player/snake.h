/*** * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * snake.h                                                *
 * Класс Snake содержит параметры игрока и методы взаимо- *
 * действия с полем. Основные параметры змеи - координаты *
 * тела и направление движения.                           *
 *                                                        *
 * Created by DSuhoi (2020) (C++11)                       *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * **/


#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "../map/map.h"

const chtype EMPTYCHR = ' ';		//пустой символ
const chtype BODYCHR = 'o' | COLOR_PAIR(GREEN);	//символ тела змеи
const chtype HEAD = '@' | COLOR_PAIR(GREEN);	//символ головы змеи
const chtype KILL = 'X' | COLOR_PAIR(RED);		//символ прохождения препятствия
const int START_SEG = 3;	//начальное кол-во сегментов
const int SEG_PLUS = 1;	//прибавление змеи

class Snake {	//класс игрока
private:
	Coords *Body;	//координаты тела змеи
	int snakeLen;	//кол-во сегментов	
	int headVect;	//направление движения змеи
	bool teleport;
public:
	Snake();
	~Snake();
	void initSnake(Map &map, bool tp); //установка начальных координат
	void endSnake();	//удаление параметров змеи
	int move(Map &map, const int bt);	//метод движения змеи
	Coords* getBody();					//указатель на координаты змеи
	Coords getBody(int B);				//координаты сегмента тела
	int getVector();					//получение направления движения
	int getSnakeLen();	//получение длины змеи
	Coords info();		//информация о координатах головы
	void killSnake(Map &map);
};

#endif
