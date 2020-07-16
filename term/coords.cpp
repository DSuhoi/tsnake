#include "coords.h"

//перегрузка операторов сравнения для координат
bool operator ==(Coords &cd1, Coords &cd2){
	return (cd1.x == cd2.x && cd1.y == cd2.y) ? true : false;
}

bool operator !=(Coords &cd1, Coords &cd2){
	return (cd1 == cd2) ? false : true;
}
