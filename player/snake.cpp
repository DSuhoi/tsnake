#include "snake.h"

// Конструктор
Snake::Snake()
{ 
	BodyCoords = nullptr;
}	

// Деструктор
Snake::~Snake()
{ 
	EraseSnake(); 
}	

void Snake::InitSnake(Coords SpawnCoords, long MaxSnakeLen, bool tp)
{
	BodyCoords = new Coords[MaxSnakeLen];
	snakeLen = START_SEG;
	BodyCoords[0] = SpawnCoords;
	headVector = KEY_RIGHT;		// По умолчанию движется вправо
	for(int i=snakeLen; i>=0; i--) BodyCoords[i] = BodyCoords[0];
}

void Snake::EraseSnake()
{
	if(BodyCoords!=nullptr){ delete [] BodyCoords; BodyCoords = NULL;}	//освобождение памяти
	
	snakeLen = START_SEG;
	headVector = 0;
}

void Snake::Move(const int vector)
{
	for(int i=snakeLen; i>0; i--){	//присваиваем новые символы дальше (для сохранения направления движения)
		BodyCoords[i] = BodyCoords[i-1];
	}
	
	switch(headVector){	//определяем направление и движимся туда 
	case KEY_LEFT:
		if(vector!=KEY_RIGHT) headVector = vector;
			BodyCoords[0].x--; break;
	case KEY_RIGHT:
		if(vector!=KEY_LEFT) headVector = vector; 
			BodyCoords[0].x++; break;
	case KEY_UP:
		if(vector!=KEY_DOWN) headVector = vector; 
			BodyCoords[0].y--; break;
	case KEY_DOWN:
		if(vector!=KEY_UP) headVector = vector; 
			BodyCoords[0].y++; break;
	default: break;
	}
	
}

void Snake::IncSnakeLen()
{
	snakeLen+=SEG_PLUS; //увеличиваем длину, если съели фрукт
	for(int seg = snakeLen; seg>snakeLen-SEG_PLUS; seg--)
		BodyCoords[seg] = BodyCoords[snakeLen-SEG_PLUS];
	 
}

Coords Snake::InfoHead(){ return BodyCoords[0]; }	//координата головы

void Snake::SetHeadCoords(int x, int y){ BodyCoords[0].x = x; BodyCoords[0].y = y; }

int Snake::GetSnakeLen(){ return snakeLen; }

Coords* Snake::GetBodyCoords(){ return BodyCoords; }

int Snake::GetVector(){ return headVector; }
