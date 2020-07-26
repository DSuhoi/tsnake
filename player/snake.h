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


const int START_SEG = 3;	// Начальное кол-во сегментов
const int SEG_PLUS = 1;		// Прибавление змеи


class Snake {	// Класс игрока
private:
	Coords *BodyCoords;	// Координаты тела змеи
	int snakeLen;		// Кол-во сегментов	
	int headVector;		// Направление движения змеи
public:
	Snake();
	~Snake();
	void InitSnake(Coords SpawnCoords, long MaxSnakeLen); // Установка начальных координат
	void EraseSnake();					// Удаление параметров змеи
	void Move(const int vector);		// Метод движения змеи
	Coords* GetBodyCoords();			// Указатель на координаты змеи
	int GetVector();		// Получение направления движения
	int GetSnakeLen();		// Получение длины змеи
	void IncSnakeLen();		// Увеличение длины змеи на определённое кол-во элементов
	Coords InfoHead();		// Информация о координатах головы
	void SetHeadCoords(int x, int y);	// Установка координат головы змеи
};

#endif
