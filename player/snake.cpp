#include "snake.h"

// Конструктор
Snake::Snake()
{ 
	BodyCoords = nullptr;	// Установка нулевого указателя
}	

// Деструктор
Snake::~Snake()
{ 
	EraseSnake(); // Удаление параметров змеи
}	

// Установка начальных координат
void Snake::InitSnake(Coords SpawnCoords, long MaxSnakeLen)
{
	// Создание змеи (координаты тела)
	BodyCoords = new Coords[MaxSnakeLen];
	snakeLen = START_SEG;
	// Установка координат появления змеи
	BodyCoords[0] = SpawnCoords;
	headVector = KEY_RIGHT;		// По умолчанию движется вправо
	for(int i = snakeLen; i >= 0; i--) BodyCoords[i] = BodyCoords[0];
}

// Удаление параметров змеи
void Snake::EraseSnake()
{
	// Удаление координат тела змеи
	if(BodyCoords!=nullptr){ 
		delete [] BodyCoords; 
		BodyCoords = nullptr;
	}	
	
	snakeLen = START_SEG;
	headVector = 0;
}

// Метод движения змеи
void Snake::Move(const int vector)
{
	// Присваиваем новые символы дальше (для сохранения направления движения)
	for(int i = snakeLen; i > 0; i--){
		BodyCoords[i] = BodyCoords[i-1];
	}
	
	// Определяем направление и движимся туда
	switch(headVector){
	case KEY_LEFT:
		if(vector != KEY_RIGHT){ 
			headVector = vector;
		}
			BodyCoords[0].x--;
			break;
	case KEY_RIGHT:
		if(vector != KEY_LEFT){ 
			headVector = vector;
		} 
			BodyCoords[0].x++; 
			break;
	case KEY_UP:
		if(vector != KEY_DOWN){ 
			headVector = vector;
		} 
			BodyCoords[0].y--; 
			break;
	case KEY_DOWN:
		if(vector != KEY_UP){ 
			headVector = vector;
		} 
			BodyCoords[0].y++; 
			break;
	default: 
			break;
	}
}

// Увеличение длины змеи на определённое кол-во элементов
void Snake::IncSnakeLen()
{
	snakeLen+=SEG_PLUS; // Увеличиваем длину, если съели фрукт
	for(int seg = snakeLen; seg > snakeLen-SEG_PLUS; seg--)
		BodyCoords[seg] = BodyCoords[snakeLen - SEG_PLUS];
	 
}

// Координата головы
Coords Snake::InfoHead()
{ 
	return BodyCoords[0]; 
}	

// Установка координат головы змеи
void Snake::SetHeadCoords(int x, int y)
{ 
	BodyCoords[0].x = x; 
	BodyCoords[0].y = y; 
}

// Получение длины змеи
int Snake::GetSnakeLen()
{ 
	return snakeLen; 
}

// Указатель на координаты змеи
Coords *Snake::GetBodyCoords()
{ 
	return BodyCoords; 
}

// Получение направления движения
int Snake::GetVector()
{ 
	return headVector; 
}
