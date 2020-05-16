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
#include "../display/display.h"

const char BORDERCHR = '#';
const chtype FRUITCHR = '$' | COLOR_PAIR(YELLOW);

//ширина и высота разных карт
const int SmallW = 40;
const int SmallH = 11;

const int MediumW = 58;
const int MediumH = 16;

const int BigW = 80;
const int BigH = 22;

//ширина и высота окна
const int WIDTH = 80;		
const int HEIGHT = 22;		


class Map {
private:
	int height, width;	//длинна и ширина карты
	WINDOW *map;	//окно карты
	Coords *border;	//указатель на координаты препятствий
	Coords *fruit;	//указатель на координаты фруктов
	int lenFruit;	//кол-во фруктов
	int lenBorder;	//кол-во препятствий
protected:
	void borderMap();	//вывод границ
public:
	Map(); 
	~Map();
	void initMap();	//настройка карты
	void selectMap(int select);	//выбор карты
	void endMap();				//удаление параметров карты
	void initFruit(int l);	//настройка количества фруктов
	void initBord(Coords snake);//настройка препятствий
	void setFruitOnMap(Coords &fr, Coords *snake, int len);	//создание фруктов
	//////////////////////////////////////////////////
	void setMap(int x, int y, chtype ch);	//установка символа на карте
	void printSubMenu(const long score,const int level, time_t &t);	//вывод подменю
	WINDOW* getMap();	//указатель на карту
	int getHeight();	//вывод высоты карты
	int getWidth();		//вывод ширины карты
	//////////////////////////////////////////////
	bool isSnake(Coords cd, Coords *snake, int len);	//проверка координат
	bool isFruit(Coords &cd);
	bool isBord(Coords cd);
	//////////////////////////////////////////////
};


#endif
