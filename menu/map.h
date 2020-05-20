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
#include "../term/display.h"

const char BORDERCHR = '#';
const chtype FRUITCHR = '$' | COLOR_PAIR(YELLOW);

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


class Coords {	//класс координат
public:
	int x;
	int y;
		//перегрузка операторов сравнения для координат
	friend bool operator ==(Coords &cd1, Coords &cd2);
	friend bool operator !=(Coords &cd1, Coords &cd2);
};


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
	void initMap();	//настройка карты
	void selectMap(int select);	//выбор карты
	void endMap();				//удаление параметров карты
	void initFruit(int l);	//настройка количества фруктов
	void initBord(Coords snake);//настройка препятствий
	void setFruitOnMap(Coords &fr, Coords *snake, int len);	//создание фруктов
	void updateMap();	//обновление изображения всех объектов карты
	//////////////////////////////////////////////////
	void setMap(int x, int y, chtype ch);	//установка символа на карте
	void printSubMenuStatic(const long lastScore, const int level);	//вывод статичной части подменю
	void printSubMenuActive(const long score, time_t &t);	//вывод обновляющейся части подменю
	void BorderCpy(int len, Coords *bd, Coords spawn);	//копирование координат
	int getHeight();	//вывод высоты карты
	int getWidth();		//вывод ширины карты
	//////////////////////////////////////////////
	Coords& setSpawnSnake();	//установка координат появления змеи
	bool isSnake(Coords cd, Coords *snake, int len);	//проверка координат
	bool isFruit(Coords &cd);
	bool isBord(Coords cd);
	//////////////////////////////////////////////
};


#endif
