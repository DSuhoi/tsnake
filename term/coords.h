#ifndef __COORDS_H__
#define __COORDS_H__

class Coords {	//класс координат
public:
	int x;
	int y;
	
		//перегрузка операторов сравнения для координат
	friend bool operator ==(Coords &cd1, Coords &cd2);
	friend bool operator !=(Coords &cd1, Coords &cd2);
};

#endif
