/*** * * * * * * * * * * * * * * * * * * * * * * * * * * **
 * map.h                                                  *
 * Класс Map содержит всю информацию о карте игры (размер,*
 * количество фруктов, препятствия). В этом классе реали- *
 * зованы проверки координат и вывод объектов на него.    *
 *                                                        *
 * Created by DSuhoi (2020) (C++11)                       *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * * **/


#ifndef __MAP_H__
#define __MAP_H__

#include <ncurses.h>
#include <ctime>
#include "../term/coords.h"
#include "../term/display.h"


const char BORDERCHR = '#';
const chtype FRUITCHR = '$' | COLOR_PAIR(YELLOW);

const chtype EMPTYCHR = ' ';		// Пустой символ
const chtype BODYCHR = 'o' | COLOR_PAIR(GREEN);	// Символ тела змеи
const chtype HEAD = '@' | COLOR_PAIR(GREEN);	// Символ головы змеи
const chtype KILL = 'X' | COLOR_PAIR(RED);		// Символ прохождения препятствия

// Ширина и высота разных карт
const int SMALL_WIDTH = 37;
const int SMALL_HEIGHT = 9;

const int MEDIUM_WIDTH = 55;
const int MEDIUM_HEIGHT = 14;

const int BIG_WIDTH = 77;
const int BIG_HEIGHT = 20;

// Ширина и высота окна
const int WIDTH = 80;
const int HEIGHT = 22;


class Map {
private:
	int height;			// Длинна карты
	int width;			// Ширина карты
	WINDOW *map;		// Окно карты
	Coords *borders;		// Указатель на координаты препятствий
	Coords *fruits;		// Указатель на координаты фруктов
	Coords spawnSnake;	// Координаты появления змеи
	int numFruits;		// Кол-во фруктов
	int numBorder;		// Кол-во препятствий

	void borderMap();	// Вывод границ
public:
	Map(); 
	~Map();
	void InitMap();				// Настройка карты
	void EraseMap();			// Удаление параметров карты
	void SelectSizeMap(int select);	// Выбор карты
	void InitBorderCoords(Coords snakeCoords);	// Настройка препятствий
	void InitFruitCoords(int number);	// Настройка количества фруктов
	void SetFruitOnMap(Coords fruitCoords, Coords *snakeCoords, int snakeLen);	// Создание фруктов
	void UpdateMap(Coords *snake, int snakeLen);	// Обновление изображения всех объектов карты

	void SetMap(int x, int y, chtype color);	// Установка символа на карте
	void PrintSubMenuStatic(const long lastScore, const int level);	// Вывод статичной части подменю
	void PrintSubMenuActive(const long score, time_t &firstTime);	// Вывод обновляющейся части подменю
	void BorderCoordsCpy(Coords *borderCoords, int numCoords, Coords spawnCoords);	// Копирование координат
	int GetHeight();	// Вывод высоты карты
	int GetWidth();		// Вывод ширины карты

	Coords GetSpawnSnake();	// Установка координат появления змеи
	bool IsSnake(Coords coords, Coords *snakeCoords, int snakeLen);	// Проверка координат игрока
	bool IsFruit(Coords coords);	// Проверка координат фруктов
	bool IsBorder(Coords coords);	// Проверка координат препятствий
};

#endif
