#include "snake.h"


Snake::Snake(){ Body = NULL; }	//конструктор

Snake::~Snake(){ endSnake(); }	//деструктор

void Snake::initSnake(Map &map, bool tp){
	Body = new Coords[map.getHeight()*map.getWidth()];
	snakeLen = START_SEG;
	Body[0] = {3, 3};
	headVect = KEY_RIGHT;	//по умолчанию движется вправо
	for(int i=snakeLen; i>=0; i--) Body[i] = Body[0];
	teleport = tp;
}

void Snake::endSnake(){
	
	if(Body!=NULL){ delete [] Body; Body = NULL;}	//освобождение памяти
	
	snakeLen = START_SEG;
	headVect = 0;
	teleport = false;
}

int Snake::move(Map &map, const int bt){
	
	map.setMap(Body[snakeLen].x,Body[snakeLen].y, EMPTYCHR);
	
	if(map.isFruit(Body[0])){ 
		snakeLen+=SEG_PLUS; //увеличиваем ее длину, если съела фрукт
		for(int seg = snakeLen; seg>snakeLen-SEG_PLUS; seg--)
			Body[seg] = Body[snakeLen-SEG_PLUS];
		map.setFruitOnMap(Body[0], Body, snakeLen);	}
	for(int i=snakeLen; i>0; i--){	//присваиваем новые символы дальше (для сохранения направления движения)
		Body[i] = Body[i-1];
		map.setMap(Body[i-1].x,Body[i-1].y, BODYCHR); }
	
	switch(headVect){	//определяем направление и движимся туда 
	case KEY_LEFT:
		if(bt!=KEY_RIGHT) headVect = bt;
			Body[0].x--; break;
	case KEY_RIGHT:
		if(bt!=KEY_LEFT) headVect = bt; 
			Body[0].x++; break;
	case KEY_UP:
		if(bt!=KEY_DOWN) headVect = bt; 
			Body[0].y--; break;
	case KEY_DOWN:
		if(bt!=KEY_UP) headVect = bt; 
			Body[0].y++; break;
	default: break;
	}
	
	if(teleport){
	if(Body[0].x == 0) Body[0].x = (map.getWidth()-1);
	else if(Body[0].x == map.getWidth()) Body[0].x = 1;
	else if(Body[0].y == 0) Body[0].y = (map.getHeight()-1);
	else if(Body[0].y == map.getHeight()) Body[0].y = 1;
	}
	
	map.setMap(Body[0].x,Body[0].y, HEAD);	//ставим символ головы
	return 0;
}

void Snake::killSnake(Map &map){ map.setMap(Body[0].x,Body[0].y, KILL); }

Coords Snake::info(){ return Body[0]; }	//координата головы

int Snake::getSnakeLen(){ return snakeLen; }

Coords* Snake::getBody(){ return Body; }

int Snake::getVector(){ return headVect; }

Coords Snake::getBody(int B){
	if(B>snakeLen) return Body[0];
	return Body[B];
}
