#include <iostream>
#include "map.h"

//конструктор
Map::Map()
{ 
	map = nullptr; 
	border = nullptr; 
	fruit = nullptr;
}	

//деструктор
Map::~Map()
{ 
	EraseMap(); 
	Display::DeleteWindow(map); 
}

//настройка карты
void Map::InitMap()
{
	int x, y;
	getmaxyx(stdscr, y, x);		
	//создание окна карты
	map = newwin(HEIGHT+2, WIDTH, (y - (HEIGHT+2))/2, (x - WIDTH)/2);
	numFruit = 0;	//установка значений
	numBorder = 0;
	spawnSnake = {3,3}; //координаты змеи
}

//выбор карты
void Map::SelectSizeMap(int select)
{
	Display::Update(map);		//обновление экрана
	switch(select){	//выбор размера поля
	case 0: width = SMALL_WIDTH; height = SMALL_HEIGHT; break;
	case 2: width = BIG_WIDTH; height = BIG_HEIGHT; break;
	case 1: 
	default: width = MEDIUM_WIDTH; height = MEDIUM_HEIGHT; break;
	};
	
	//создание границ
	Display::PrintScr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
	
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
void Map::EraseMap()
{
	//проверка указателей и освобождение памяти
	if(fruit!=nullptr){ delete [] fruit; fruit = nullptr; }
	if(border!=nullptr){ delete [] border; border = nullptr; }
	
	width = 0;	//сброс ширины и высоты поля
	height = 0;
	spawnSnake = {3,3};
	numFruit = 0; //сброс параметров кол-ва фруктов и препятствий
	numBorder = 0;
	
	Display::Update(map);
}

//настройка фруктов
void Map::InitFruitCoords(int l)
{
	if(l<100 || l>0) numFruit = l;	//если не больше 100
	else numFruit = 1;
	
	fruit = new Coords[numFruit];	//выделяем память под координаты фруктов
	
	
	
	std::srand(unsigned(std::time(0)));	//генерируем числа из даты
	Coords randomCoords;	//координаты фрукта
	for(int i=0; i<numFruit; i++){
     do{	 
		randomCoords.x = std::rand()% width;	//случайные координаты
		randomCoords.y = std::rand()% height;
		}while(randomCoords.x < 1 || randomCoords.y < 1 ||
		 IsFruit(randomCoords) || IsBord(randomCoords));
	fruit[i] = randomCoords;	//проверка и присвоение координат
	SetMap(fruit[i].x,fruit[i].y,FRUITCHR);}
}

//настройка препятствий
void Map::InitBorderCoords(Coords snake)
{
	numBorder = height*width/20;	//кол-во препятствий
	
	border = new Coords[numBorder];	//выделяем память под координаты препятствий
	
	std::srand(unsigned(std::time(0)));	//генерируем числа из даты
	Coords randomCoords;
	for(int i = 0; i<numBorder; i++){	
		do{
			randomCoords.x = std::rand()% width;	//генерируем и проверяем координаты
			randomCoords.y = std::rand()% height;	//не ближе 5 блоков до начального положения змеи
	   }while(IsBord(randomCoords) || 
		((snake.x-3) < randomCoords.x  && randomCoords.x < (snake.x+5) && randomCoords.y == snake.y ) ||
		 randomCoords.x < 1 || randomCoords.y < 1);
		border[i] = randomCoords;	//присвоение и вывод препятствий по координатам
	}
}

//создание фруктов
void Map::SetFruitOnMap(Coords fr, Coords *snake, int len)
{	
	std::srand(unsigned(std::time(0)));	//генерируем числа по времени
	int errorCounter = 0;	//счётчик повторений
	Coords randomCoords;
	for(int set=0; set<numFruit;set++)
		if(fr == fruit[set]){
			do{	
				if(errorCounter++>1000) return; //если прошло больше 1000 повторений, то выходим из цикла
				randomCoords.x = std::rand()% width;	//случайные координаты
				randomCoords.y = std::rand()% height;
				//проверка координат
			}while(IsSnake(randomCoords,snake,len) || 
					randomCoords.x < 1 || randomCoords.y < 1 || 
						IsFruit(randomCoords) || IsBord(randomCoords));
			fruit[set] = randomCoords;	//присвоение корректных координат
			SetMap(fruit[set].x,fruit[set].y,FRUITCHR);	//и вывод фрукта
	}
}

//копирование координат
void Map::BorderCoordsCpy(int len, Coords *bd, Coords spawn)
{
	numBorder = len;	//если кол-во координат больше длины массива
	if(border==nullptr) border = new Coords[numBorder];	
	for(int i=0; i<numBorder; i++)
		border[i] = bd[i];
	spawnSnake = spawn;
}

//обновление изображения всех объектов карты
void Map::UpdateMap(Coords *snake, int snakeLen)
{
	for(int i=0; i<=width;i++){
		SetMap(i, 0, BORDERCHR);
		SetMap(i, height, BORDERCHR); }
	
	for(int i=0; i<=height;i++){
		SetMap(0, i, BORDERCHR);
		SetMap(width, i, BORDERCHR); }
	
	for(int i=0; i<numBorder; i++)
		SetMap(border[i].x, border[i].y, BORDERCHR);
	
	for(int i=0; i<numFruit; i++)
		SetMap(fruit[i].x, fruit[i].y, FRUITCHR);
	
	SetMap(snake[snakeLen].x, snake[snakeLen].y, EMPTYCHR);	//очистка хвоста змеи
	
	for(int i=snakeLen; i>0; i--)
		SetMap(snake[i-1].x, snake[i-1].y, BODYCHR);
	
	SetMap(snake[0].x, snake[0].y, HEAD);	//ставим символ головы
}

////////////////////////////////////////////////////////////////////////

//проверки координат поля
bool Map::IsSnake(Coords cd, Coords *snake, int len)
{
	for(int i=len; i>0; i--)
		if(cd == snake[i]) return true;
	return false;
}

bool Map::IsFruit(Coords cd)
{
	for(int i=0; i<numFruit; i++)
		if(cd == fruit[i]) return true;
	return false;
}

bool Map::IsBord(Coords cd)
{
	if(border==nullptr) return false;
	for(int i=0; i<numBorder; i++)
		if(cd == border[i]) return true;
	return false;
}

//вывод статичной части подменю
void Map::PrintSubMenuStatic(const long lastScore, const int level)
{
	char buffLastScore[15];	//массив для рекорда
	char buffLevel[10];	//массив для уровня
	sprintf(buffLastScore,"Record: %0*ld", 6, lastScore);
	sprintf(buffLevel,"Level: %d", level);
	Display::PrintScr(map, WIDTH/2 - 18, HEIGHT, buffLastScore, RED);
	Display::PrintScr(map, WIDTH/2 + 5, HEIGHT, buffLevel,GREEN);
}

//вывод обновляющейся части подменю
void Map::PrintSubMenuActive(const long score, time_t &t)
{
	int allTime = time(0) - t;	//все время с начала игры
	unsigned int sec = allTime % 60;		//секунды
	unsigned int min = allTime / 60;		//минуты
	char buffScore[14];	//массив для счёта
	char buffTime[15];	//массив для времени
	sprintf(buffScore,"Score: %0*ld", 6, score);
	sprintf(buffTime,"Time: %0*d:%0*d", 2, min, 2, sec);
	Display::PrintScr(map, 2, HEIGHT, buffScore,YELLOW);
	Display::PrintScr(map, WIDTH - 15, HEIGHT, buffTime,BLUE);
}


//устанавливаем объект на карту
void Map::SetMap(int x, int y, chtype ch)
{
	Display::PrintScr(map,((WIDTH-width)/2) + x, ((HEIGHT - height)/2) + y,ch);
}

////////////////////////////////////////////////////////////////////////
//установка координат появления змеи
Coords Map::GetSpawnSnake()
{ 
	return spawnSnake; 
}

//вернуть высоту карты
int Map::GetHeight()
{ 
	return height; 
}

//вернуть длину карты
int Map::GetWidth()
{ 
	return width; 
}	
////////////////////////////////////////////////////////////////////////
