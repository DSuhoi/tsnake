#include <iostream>
#include "map.h"

// Конструктор
Map::Map()
{ 
	map = nullptr; 
	borders = nullptr; 
	fruits = nullptr;
}	

// Деструктор
Map::~Map()
{ 
	EraseMap(); 
	Display::DeleteWindow(map); 
}

// Настройка карты
void Map::InitMap()
{
	int x, y;
	getmaxyx(stdscr, y, x);		
	// Создание окна карты
	map = newwin(HEIGHT + 2, WIDTH, (y - (HEIGHT + 2))/2, (x - WIDTH)/2);
	numFruits = 0;	// Установка значений
	numBorder = 0;
	spawnSnake = {3, 3}; // Координаты змеи
}

// Выбор карты
void Map::SelectSizeMap(int select)
{
	Display::Update(map);		// Обновление экрана
	switch(select){	// Выбор размера поля
	case 0: 
		width = SMALL_WIDTH; 
		height = SMALL_HEIGHT; 
		break;
	case 2: 
		width = BIG_WIDTH; 
		height = BIG_HEIGHT; 
		break;
	case 1: 
	default: 
		width = MEDIUM_WIDTH; 
		height = MEDIUM_HEIGHT; 
		break;
	};
	
	// Создание границ
	Display::PrintScr(map,WIDTH - 9, 0,(char*)"TSNAKE", BLUE);
	
	for(int i = 0; i <= width;i++){
		SetMap(i, 0, BORDERCHR);
		SetMap(i, height, BORDERCHR);
	}
	
	for(int i = 0; i <= height;i++){
		SetMap(0, i, BORDERCHR);
		SetMap(width, i, BORDERCHR);
	}
}

// Удаление параметров карты
void Map::EraseMap()
{
	// Проверка указателей и освобождение памяти
	if(fruits != nullptr){ 
		delete [] fruits; 
		fruits = nullptr; 
	}
	
	if(borders != nullptr){ 
		delete [] borders; 
		borders = nullptr; 
	}
	
	width = 0;	// Сброс ширины и высоты поля
	height = 0;
	spawnSnake = {3, 3};
	numFruits = 0; // Сброс параметров кол-ва фруктов и препятствий
	numBorder = 0;
	// Обновление карты
	Display::Update(map);
}

// Настройка фруктов
void Map::InitFruitCoords(int number)
{
	// Проверка кол-ва фруктов
	if(number < 100 || number > 0){ 
		numFruits = number;	
	}
	else{ 
		numFruits = 1;
	}
	
	fruits = new Coords[numFruits];	// Выделяем память под координаты фруктов
	std::srand(unsigned(std::time(0)));	// Генерируем числа из даты
	Coords randomCoords;	// Координаты фрукта
	for(int i = 0; i < numFruits; i++){
		do{	 
			randomCoords.x = std::rand()% width;	// Случайные координаты
			randomCoords.y = std::rand()% height;
		}while(randomCoords.x < 1 || randomCoords.y < 1 ||
			IsFruit(randomCoords) || IsBorder(randomCoords));
		fruits[i] = randomCoords;	// Проверка и присвоение координат
		SetMap(fruits[i].x,fruits[i].y,FRUITCHR);	// Постановка фрукта на карте
	}
}

// Настройка препятствий
void Map::InitBorderCoords(Coords snake)
{
	numBorder = height*width/20;	// Кол-во препятствий
	
	borders = new Coords[numBorder];	// Выделяем память под координаты препятствий
	
	std::srand(unsigned(std::time(0)));	// Генерируем числа из даты
	Coords randomCoords;
	for(int i = 0; i < numBorder; i++){	
		do{
			randomCoords.x = std::rand()% width;	// Генерируем и проверяем координаты
			randomCoords.y = std::rand()% height;	// Не ближе 5 блоков до начального положения змеи
	   }while(IsBorder(randomCoords) || 
		((snake.x-3) < randomCoords.x  && randomCoords.x < (snake.x+5) && randomCoords.y == snake.y ) ||
		 randomCoords.x < 1 || randomCoords.y < 1);
		borders[i] = randomCoords;	// Присвоение и вывод препятствий по координатам
	}
}

// Создание фруктов
void Map::SetFruitOnMap(Coords fruitCoords, Coords *snake, int number)
{	
	std::srand(unsigned(std::time(0)));	// Генерируем числа по времени
	int errorCounter = 0;	// Счётчик повторений
	Coords randomCoords;
	for(int set = 0; set < numFruits;set++){
		if(fruitCoords == fruits[set]){
			do{	
				if(errorCounter++ > 1000) return; // Если прошло больше 1000 повторений, то выходим из цикла
				randomCoords.x = std::rand()% width;	// Случайные координаты
				randomCoords.y = std::rand()% height;
				// Проверка координат
			}while(IsSnake(randomCoords,snake, number) || 
					randomCoords.x < 1 || randomCoords.y < 1 || 
						IsFruit(randomCoords) || IsBorder(randomCoords));
			fruits[set] = randomCoords;	// Присвоение корректных координат
			SetMap(fruits[set].x,fruits[set].y,FRUITCHR);	// и вывод фрукта
		}
	}
}

// Копирование координат
void Map::BorderCoordsCpy(Coords *borderCoords, int numCoords, Coords spawnCoords)
{
	numBorder = numCoords;	// Если кол-во координат больше длины массива
	if(borders == nullptr){
		borders = new Coords[numBorder];
	}
	
	for(int i = 0; i < numBorder; i++){
		borders[i] = borderCoords[i];
	}
	
	spawnSnake = spawnCoords;
}

// Обновление изображения всех объектов карты
void Map::UpdateMap(Coords *snake, int snakeLen)
{
	// Обновление границ карты
	for(int i = 0; i <= width;i++){
		SetMap(i, 0, BORDERCHR);
		SetMap(i, height, BORDERCHR); 
	}
	
	for(int i = 0; i <= height;i++){
		SetMap(0, i, BORDERCHR);
		SetMap(width, i, BORDERCHR); 
	}
	// Обновление препятствий
	for(int i = 0; i < numBorder; i++){
		SetMap(borders[i].x, borders[i].y, BORDERCHR);
	}
	// Обновление фруктов
	for(int i = 0; i < numFruits; i++){
		SetMap(fruits[i].x, fruits[i].y, FRUITCHR);
	}
	
	SetMap(snake[snakeLen].x, snake[snakeLen].y, EMPTYCHR);	// Очистка хвоста змеи
	// Обновление тела змеи
	for(int i = snakeLen; i > 0; i--){
		SetMap(snake[i-1].x, snake[i-1].y, BODYCHR);
	}
	
	SetMap(snake[0].x, snake[0].y, HEAD);	// Ставим символ головы
}

////////////////////////////////////////////////////////////////////////

// Проверки координат поля
bool Map::IsSnake(Coords coord, Coords *snake, int snakeLen)
{
	// 
	for(int i = snakeLen; i > 0; i--){
		if(coord == snake[i]){
			return true;
		}
	}
	
	return false;
}

// Проверка координат фруктов
bool Map::IsFruit(Coords coord)
{
	for(int i = 0; i < numFruits; i++){
		if(coord == fruits[i]){
			return true;
		}
	}
	
	return false;
}

// Проверка координат препятствий
bool Map::IsBorder(Coords coord)
{
	// Проверка на наличие препятствий
	if(borders == nullptr){
		return false;
	}
	
	for(int i = 0; i < numBorder; i++){
		if(coord == borders[i]){ 
			return true;
		}
	}
	
	return false;
}

// Вывод статичной части подменю
void Map::PrintSubMenuStatic(const long lastScore, const int level)
{
	char buffLastScore[15];	// Массив для рекорда
	char buffLevel[10];		// Массив для уровня
	sprintf(buffLastScore,"Record: %0*ld", 6, lastScore);
	sprintf(buffLevel,"Level: %d", level);
	Display::PrintScr(map, WIDTH/2 - 18, HEIGHT, buffLastScore, RED);
	Display::PrintScr(map, WIDTH/2 + 5, HEIGHT, buffLevel,GREEN);
}

// Вывод обновляющейся части подменю
void Map::PrintSubMenuActive(const long score, time_t &firstTime)
{
	int allTime = time(0) - firstTime;	// Все время с начала игры
	unsigned int sec = allTime % 60;		// Секунды
	unsigned int min = allTime / 60;		// Минуты
	char buffScore[14];	// Массив для счёта
	char buffTime[15];	// Массив для времени
	sprintf(buffScore,"Score: %0*ld", 6, score);
	sprintf(buffTime,"Time: %0*d:%0*d", 2, min, 2, sec);
	Display::PrintScr(map, 2, HEIGHT, buffScore,YELLOW);
	Display::PrintScr(map, WIDTH - 15, HEIGHT, buffTime,BLUE);
}


// Устанавливаем объект на карту
void Map::SetMap(int x, int y, chtype color)
{
	Display::PrintScr(map,((WIDTH-width)/2) + x, ((HEIGHT - height)/2) + y, color);
}

////////////////////////////////////////////////////////////////////////
// Установка координат появления змеи
Coords Map::GetSpawnSnake()
{ 
	return spawnSnake; 
}

// Вернуть высоту карты
int Map::GetHeight()
{ 
	return height; 
}

// Вернуть длину карты
int Map::GetWidth()
{ 
	return width; 
}
