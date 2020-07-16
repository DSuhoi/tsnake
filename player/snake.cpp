#include "snake.h"


Snake::Snake(){ Body = NULL; }	//конструктор

Snake::~Snake(){ EndSnake(); }	//деструктор

void Snake::InitSnake(Coords SpawnCoords, long MaxSnakeLen, bool tp){
	Body = new Coords[MaxSnakeLen];
	snakeLen = START_SEG;
	Body[0] = SpawnCoords;
	headVect = KEY_RIGHT;	//по умолчанию движется вправо
	for(int i=snakeLen; i>=0; i--) Body[i] = Body[0];
}

void Snake::EndSnake(){
	
	if(Body!=NULL){ delete [] Body; Body = NULL;}	//освобождение памяти
	
	snakeLen = START_SEG;
	headVect = 0;
}

void Snake::Move(const int vector){
	
	for(int i=snakeLen; i>0; i--){	//присваиваем новые символы дальше (для сохранения направления движения)
		Body[i] = Body[i-1];
	}
	
	switch(headVect){	//определяем направление и движимся туда 
	case KEY_LEFT:
		if(vector!=KEY_RIGHT) headVect = vector;
			Body[0].x--; break;
	case KEY_RIGHT:
		if(vector!=KEY_LEFT) headVect = vector; 
			Body[0].x++; break;
	case KEY_UP:
		if(vector!=KEY_DOWN) headVect = vector; 
			Body[0].y--; break;
	case KEY_DOWN:
		if(vector!=KEY_UP) headVect = vector; 
			Body[0].y++; break;
	default: break;
	}
	
}

void Snake::IncSnakeLen(){
	snakeLen+=SEG_PLUS; //увеличиваем длину, если съели фрукт
	for(int seg = snakeLen; seg>snakeLen-SEG_PLUS; seg--)
		Body[seg] = Body[snakeLen-SEG_PLUS];
	 
}

Coords Snake::Info(){ return Body[0]; }	//координата головы

void Snake::SetHead(int x, int y){ Body[0].x = x; Body[0].y = y; }

int Snake::GetSnakeLen(){ return snakeLen; }

Coords* Snake::GetBody(){ return Body; }

int Snake::GetVector(){ return headVect; }
