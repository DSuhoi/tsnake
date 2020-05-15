#include <iostream>
#include <ncurses.h>
#include <ctime>
#include "map.h"
#include "../display/display.h"

//настройка карты
void Map::initMap(){
	int x, y;
	getmaxyx(stdscr, y, x);		
	//создание окна карты
	map = newwin(HEIGHT+2, WIDTH, (y - (HEIGHT+2))/2, (x - WIDTH)/2);
	lenFructs = 0;	//установка значений
	lenBorder = 0;
}

//выбор карты
void Map::selectMap(int select){
	update(map);		//обновление экрана
	switch(select){	//выбор размера поля
	case 0: width = SmallW; height = SmallH; break;
	case 1: width = MediumW; height = MediumH; break;
	case 2: width = BigW; height = BigH; break;
	};
	
	//создание границ
	Display::printScr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
	
	for(int i=1; i<width-1;i++){
		setMap(i, 1, BORDERCHR);
		setMap(i, height-1, BORDERCHR);
	}
	
	for(int i=1; i<height-1;i++){
		setMap(1, i, BORDERCHR);
		setMap(width-2, i, BORDERCHR);
	}
}

//удаление параметров карты
void Map::endMap(){
	
	//проверка указателей и освобождение памяти
	if(fructs!=NULL){ delete [] fructs; fructs = NULL; }
	if(border!=NULL){ delete [] border; border = NULL; }
	
	width = 0;	//сброс ширины и высоты поля
	height = 0;
	
	lenFructs = 0; //сброс параметров кол-ва фруктов и препятствий
	lenBorder = 0;
	
	cls(map);
	update();
}

//настройка фруктов
void Map::initFruct(int l){
	
	if(l<100 || l>0) lenFructs = l;	//если не больше 100
	else lenFructs = 1;
	
	fructs = new Coords[lenFructs];	//выделяем память под координаты фруктов
	
	Coords r;	//координаты фрукта
	
	std::srand(unsigned(std::time(0)));	//генерируем числа из даты
	
	for(int i=0; i<lenFructs; i++){
     do{	 
		r.x = std::rand()% (width-2);	//случайные координаты
		r.y = std::rand()% (height-1);
		}while(r.x < 2 || r.y < 2 || isFruct(r));
	fructs[i] = r;	//проверка и присвоение координат
	setMap(fructs[i].x,fructs[i].y,FRUCTCHR);}
}

//настройка препятствий
void Map::initBord(Coords snake){
	
	lenBorder = height*width/20;	//кол-во препятствий
	
	border = new Coords[lenBorder];	//выделяем память под координаты препятствий
	
	Coords r;
	
	std::srand(unsigned(std::time(0)));	//генерируем числа из даты
	
	for(int i = 0; i<lenBorder; i++){	
	do{
		r.x = std::rand()% (width-2);	//генерируем и проверяем координаты
		r.y = std::rand()% (height-1);	//не ближе 5 блоков до начального положения змеи
	   }while(isBord(r) || (r.x < (snake.x+5) && r.x > (snake.x-3) && r.y == snake.y ) || r.x < 2 || r.y < 2 || isFruct(r));
	border[i] = r;	//присвоение и вывод препятствий по координатам
	setMap(border[i].x,border[i].y,BORDERCHR); }
}

//создание фруктов
void Map::setFructOnMap(Coords &fr, Coords *snake, int len){
	
	Coords r;
	
	std::srand(unsigned(std::time(0)));	//генерируем числа по времени
	int errCnt = 0;	//счётчик повторений
	for(int set=0; set<lenFructs;set++)
		if(fr == fructs[set]){
     do{	
		if(errCnt++>1000) return; //если прошло больше 1000 повторений, то выходим из цикла
		r.x = std::rand()% (width-2);	//случайные координаты
		r.y = std::rand()% (height-1);
		//проверка координат
		}while(isSnake(r,snake,len) || r.x < 2 || r.y < 2 || isFruct(r) || isBord(r));
	fructs[set] = r;	//присвоение корректных координат
	setMap(fructs[set].x,fructs[set].y,FRUCTCHR);	//и вывод фрукта
	}
}
////////////////////////////////////////////////////////////////////////

//проверки координат поля
bool Map::isSnake(Coords cd, Coords *snake, int len){
	for(int i=len; i>0; i--)
		if(cd == snake[i]) return true;
	return false;
}

bool Map::isFruct(Coords &cd){
	for(int i=0; i<lenFructs; i++)
		if(cd == fructs[i]) return true;
	return false;
}

bool Map::isBord(Coords cd){
	for(int i=0; i<lenBorder; i++)
		if(cd == border[i]) return true;
	return false;
}

//печатаем подменю карты
void Map::printSubMenu(const long score,const int level, time_t &t){
	int allTime = time(0) - t;	//все время с начала игры
	int sec = allTime % 60;		//секунды
	int min = allTime / 60;		//минуты
	char buffScore[16];	//массив для счёта
	char buffLevel[10];	//массив для уровня
	char buffTime[20];	//массив для времени
	sprintf(buffScore,"Score: %0*ld",8, score);
	sprintf(buffLevel,"Level: %d", level);
	sprintf(buffTime,"Time: %0*d:%0*d",2, min, 2, sec);
	Display::printScr(map, 2, HEIGHT, buffScore,YELLOW);
	Display::printScr(map, WIDTH/2 - 4, HEIGHT, buffLevel,GREEN);
	Display::printScr(map, WIDTH - 15, HEIGHT, buffTime,BLUE);
}


//устанавливаем объект на карту
void Map::setMap(int x, int y, chtype ch){
	Display::printScr(map,((WIDTH-width)/2) + x, ((HEIGHT - height)/2) + y,ch);
}

////////////////////////////////////////////////////////////////////////
int Map::getHeight(){ return height; }	//вернуть высоту карты
int Map::getWidth(){ return width; }	//вернуть длину карты
WINDOW* Map::getMap(){ return map; }	//возвращаем указатель на окно карты
////////////////////////////////////////////////////////////////////////



