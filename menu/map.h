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

const chtype EMPTYCHR = ' ';		//пустой символ
const chtype BODYCHR = 'o' | COLOR_PAIR(GREEN);	//символ тела змеи
const chtype HEAD = '@' | COLOR_PAIR(GREEN);	//символ головы змеи
const chtype KILL = 'X' | COLOR_PAIR(RED);		//символ прохождения препятствия

//ширина и высота разных карт
const int SMALL_WIDTH = 37;
const int SMALL_HEIGHT = 9;

const int MEDIUM_WIDTH = 55;
const int MEDIUM_HEIGHT = 14;

const int BIG_WIDTH = 77;
const int BIG_HEIGHT = 20;

//ширина и высота окна
const int WIDTH = 80;		
const int HEIGHT = 22;		


class Map {
private:
	int height;		// Длинна карты
	int width;		// Ширина карты
	WINDOW *map;	// Окно карты
	Coords *border;	// Указатель на координаты препятствий
	Coords *fruit;	// Указатель на координаты фруктов
	Coords spawnSnake;	// Координаты появления змеи
	int numFruit;	// Кол-во фруктов
	int numBorder;	// Кол-во препятствий
protected:
	void borderMap();	// Вывод границ
public:
	Map(); 
	~Map();
	void InitMap();				// Настройка карты
	void EraseMap();			// Удаление параметров карты
	void SelectSizeMap(int select);	// Выбор карты
	void InitBorderCoords(Coords snake);// Настройка препятствий
	void InitFruitCoords(int l);	// Настройка количества фруктов
	void SetFruitOnMap(Coords fr, Coords *snake, int len);	// Создание фруктов
	void UpdateMap(Coords *snake, int snakeLen);	// Обновление изображения всех объектов карты
	//////////////////////////////////////////////////
	void SetMap(int x, int y, chtype ch);	// Установка символа на карте
	void PrintSubMenuStatic(const long lastScore, const int level);	// Вывод статичной части подменю
	void PrintSubMenuActive(const long score, time_t &t);	// Вывод обновляющейся части подменю
	void BorderCoordsCpy(int len, Coords *bd, Coords spawn);	// Копирование координат
	int GetHeight();	// Вывод высоты карты
	int GetWidth();		// Вывод ширины карты
	//////////////////////////////////////////////
	Coords GetSpawnSnake();	// Установка координат появления змеи
	bool IsSnake(Coords cd, Coords *snake, int len);	// Проверка координат игрока
	bool IsFruit(Coords cd);	// Проверка координат фруктов
	bool IsBord(Coords cd);		// Проверка координат препятствий
	//////////////////////////////////////////////
};

#endif
