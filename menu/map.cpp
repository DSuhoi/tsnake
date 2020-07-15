#include <iostream>
#include <ncurses.h>
#include <ctime>
#include "map.h"


//перегрузка операторов сравнения для координат
bool operator ==(Coords &cd1, Coords &cd2){
	return (cd1.x == cd2.x && cd1.y == cd2.y) ? true : false;
}

bool operator !=(Coords &cd1, Coords &cd2){
	return (cd1 == cd2) ? false : true;
}


Map::Map(){ map = NULL; border = NULL; fruit = NULL;}	//конструктор

Map::~Map(){ EndMap(); deleteWindow(map); }	//деструктор

//настройка карты
void Map::InitMap(){
	int x, y;
	getmaxyx(stdscr, y, x);		
	//создание окна карты
	map = newwin(HEIGHT+2, WIDTH, (y - (HEIGHT+2))/2, (x - WIDTH)/2);
	lenFruit = 0;	//установка значений
	lenBorder = 0;
	spawnSnake = {3,3}; //координаты змеи
}

//выбор карты
void Map::SelectMap(int select){
	update(map);		//обновление экрана
	switch(select){	//выбор размера поля
	case 0: width = SmallW; height = SmallH; break;
	case 2: width = BigW; height = BigH; break;
	case 1: 
	default: width = MediumW; height = MediumH; break;
	};
	
	//создание границ
	printScr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
	
	for(int i=0; i<=width;i++){
		SetMap(i, 0, BORDERCHR);
		SetMap(i, height, BORDERCHR);
	}
	
	for(int i=0; i<=height;i++){
		SetMap(0, i, BORDERCHR);
		SetMap(width, i, BORDERCHR);
	}
}

//удаление параметров карты
void Map::EndMap(){
	
	//проверка указателей и освобождение памяти
	if(fruit!=NULL){ delete [] fruit; fruit = NULL; }
	if(border!=NULL){ delete [] border; border = NULL; }
	
	width = 0;	//сброс ширины и высоты поля
	height = 0;
	spawnSnake = {3,3};
	lenFruit = 0; //сброс параметров кол-ва фруктов и препятствий
	lenBorder = 0;
	
	update(map);
}

//настройка фруктов
void Map::InitFruit(int l){
	
	if(l<100 || l>0) lenFruit = l;	//если не больше 100
	else lenFruit = 1;
	
	fruit = new Coords[lenFruit];	//выделяем память под координаты фруктов
	
	Coords r;	//координаты фрукта
	
	std::srand(unsigned(std::time(0)));	//генерируем числа из даты
	
	for(int i=0; i<lenFruit; i++){
     do{	 
		r.x = std::rand()% width;	//случайные координаты
		r.y = std::rand()% height;
		}while(r.x < 1 || r.y < 1 || IsFruit(r) || IsBord(r));
	fruit[i] = r;	//проверка и присвоение координат
	SetMap(fruit[i].x,fruit[i].y,FRUITCHR);}
}

//настройка препятствий
void Map::InitBord(Coords snake){
	
	lenBorder = height*width/20;	//кол-во препятствий
	
	border = new Coords[lenBorder];	//выделяем память под координаты препятствий
	
	Coords r;
	
	std::srand(unsigned(std::time(0)));	//генерируем числа из даты
	
	for(int i = 0; i<lenBorder; i++){	
	do{
		r.x = std::rand()% width;	//генерируем и проверяем координаты
		r.y = std::rand()% height;	//не ближе 5 блоков до начального положения змеи
	   }while(IsBord(r) || (r.x < (snake.x+5) && r.x > (snake.x-3) && r.y == snake.y ) || r.x < 1 || r.y < 1);
	border[i] = r;	//присвоение и вывод препятствий по координатам
	}
}

//создание фруктов
void Map::SetFruitOnMap(Coords &fr, Coords *snake, int len){
	
	Coords r;
	
	std::srand(unsigned(std::time(0)));	//генерируем числа по времени
	int errCnt = 0;	//счётчик повторений
	for(int set=0; set<lenFruit;set++)
		if(fr == fruit[set]){
     do{	
		if(errCnt++>1000) return; //если прошло больше 1000 повторений, то выходим из цикла
		r.x = std::rand()% width;	//случайные координаты
		r.y = std::rand()% height;
		//проверка координат
		}while(IsSnake(r,snake,len) || r.x < 1 || r.y < 1 || IsFruit(r) || IsBord(r));
	fruit[set] = r;	//присвоение корректных координат
	SetMap(fruit[set].x,fruit[set].y,FRUITCHR);	//и вывод фрукта
	}
}

//копирование координат
void Map::BorderCpy(int len, Coords *bd, Coords spawn){
	lenBorder = len;	//если кол-во координат больше длины массива
	if(border==NULL) border = new Coords[lenBorder];	
	for(int i=0; i<lenBorder; i++)
		border[i] = bd[i];
	spawnSnake = spawn;
}

//обновление изображения всех объектов карты
void Map::UpdateMap(){
	for(int i=0; i<=width;i++){
		SetMap(i, 0, BORDERCHR);
		SetMap(i, height, BORDERCHR); }
	
	for(int i=0; i<=height;i++){
		SetMap(0, i, BORDERCHR);
		SetMap(width, i, BORDERCHR); }
	
	for(int i=0; i<lenBorder; i++)
		SetMap(border[i].x, border[i].y, BORDERCHR);
	
	for(int i=0; i<lenFruit; i++)
		SetMap(fruit[i].x, fruit[i].y, FRUITCHR);
}

////////////////////////////////////////////////////////////////////////

//проверки координат поля
bool Map::IsSnake(Coords cd, Coords *snake, int len){
	for(int i=len; i>0; i--)
		if(cd == snake[i]) return true;
	return false;
}

bool Map::IsFruit(Coords &cd){
	for(int i=0; i<lenFruit; i++)
		if(cd == fruit[i]) return true;
	return false;
}

bool Map::IsBord(Coords cd){
	if(border==NULL) return false;
	for(int i=0; i<lenBorder; i++)
		if(cd == border[i]) return true;
	return false;
}

//вывод статичной части подменю
void Map::PrintSubMenuStatic(const long lastScore, const int level){
	char buffLastScore[15];	//массив для рекорда
	char buffLevel[10];	//массив для уровня
	sprintf(buffLastScore,"Record: %0*ld", 6, lastScore);
	sprintf(buffLevel,"Level: %d", level);
	printScr(map, WIDTH/2 - 18, HEIGHT, buffLastScore, RED);
	printScr(map, WIDTH/2 + 5, HEIGHT, buffLevel,GREEN);
}

//вывод обновляющейся части подменю
void Map::PrintSubMenuActive(const long score, time_t &t){
	int allTime = time(0) - t;	//все время с начала игры
	unsigned int sec = allTime % 60;		//секунды
	unsigned int min = allTime / 60;		//минуты
	char buffScore[14];	//массив для счёта
	char buffTime[15];	//массив для времени
	sprintf(buffScore,"Score: %0*ld", 6, score);
	sprintf(buffTime,"Time: %0*d:%0*d", 2, min, 2, sec);
	printScr(map, 2, HEIGHT, buffScore,YELLOW);
	printScr(map, WIDTH - 15, HEIGHT, buffTime,BLUE);
}


//устанавливаем объект на карту
void Map::SetMap(int x, int y, chtype ch){
	printScr(map,((WIDTH-width)/2) + x, ((HEIGHT - height)/2) + y,ch);
}

////////////////////////////////////////////////////////////////////////
Coords& Map::SetSpawnSnake(){ return spawnSnake; }	//установка координат появления змеи
int Map::GetHeight(){ return height; }	//вернуть высоту карты
int Map::GetWidth(){ return width; }	//вернуть длину карты
////////////////////////////////////////////////////////////////////////



