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

#include <ncurses.h>
#include "../term/coords.h"
#include "../term/display.h"


const int START_SEG = 3;	//начальное кол-во сегментов
const int SEG_PLUS = 1;	//прибавление змеи

class Snake {	//класс игрока
private:
	Coords *Body;	//координаты тела змеи
	int snakeLen;	//кол-во сегментов	
	int headVect;	//направление движения змеи
public:
	Snake();
	~Snake();
	void InitSnake(Coords SpawnCoords, long MaxSnakeLen, bool tp); //установка начальных координат
	void EndSnake();	//удаление параметров змеи
	void Move(const int vector);		//метод движения змеи
	Coords* GetBody();					//указатель на координаты змеи
	int GetVector();					//получение направления движения
	int GetSnakeLen();	//получение длины змеи
	void IncSnakeLen();	//увеличение длины змеи на определённое кол-во элементов
	Coords Info();		//информация о координатах головы
	void SetHead(int x, int y);	//установка координат головы змеи
};

#endif
