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
const int SmallW = 37;
const int SmallH = 9;

const int MediumW = 55;
const int MediumH = 14;

const int BigW = 77;
const int BigH = 20;

//ширина и высота окна
const int WIDTH = 80;		
const int HEIGHT = 22;		


class Map {
private:
	int height, width;	//длинна и ширина карты
	WINDOW *map;	//окно карты
	Coords *border;	//указатель на координаты препятствий
	Coords *fruit;	//указатель на координаты фруктов
	Coords spawnSnake;	//координаты появления змеи
	int lenFruit;	//кол-во фруктов
	int lenBorder;	//кол-во препятствий
protected:
	void borderMap();	//вывод границ
public:
	Map(); 
	~Map();
	void InitMap();	//настройка карты
	void EndMap();				//удаление параметров карты
	void SelectMap(int select);	//выбор карты
	void InitBord(Coords snake);//настройка препятствий
	void InitFruit(int l);	//настройка количества фруктов
	void SetFruitOnMap(Coords fr, Coords *snake, int len);	//создание фруктов
	void UpdateMap(Coords *snake, int snakeLen);	//обновление изображения всех объектов карты
	//////////////////////////////////////////////////
	void SetMap(int x, int y, chtype ch);	//установка символа на карте
	void PrintSubMenuStatic(const long lastScore, const int level);	//вывод статичной части подменю
	void PrintSubMenuActive(const long score, time_t &t);	//вывод обновляющейся части подменю
	void BorderCpy(int len, Coords *bd, Coords spawn);	//копирование координат
	int GetHeight();	//вывод высоты карты
	int GetWidth();		//вывод ширины карты
	//////////////////////////////////////////////
	Coords GetSpawnSnake();	//установка координат появления змеи
	bool IsSnake(Coords cd, Coords *snake, int len);	//проверка координат
	bool IsFruit(Coords cd);
	bool IsBord(Coords cd);
	//////////////////////////////////////////////
};


#endif
