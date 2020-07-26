#include <fstream>
#include "files.h"



//запись результата игры в файл
void SaveRecords(long *score){
	std::ofstream fout("data/score.tsn", std::ios::binary);
	for(int i=0; i<30; i++)	// 3 типа карты * 10 уровней скорости = 30 
		fout.write((char*)&score[i],sizeof(score[i]));	//пишем всё обратно
	fout.close();
}

//получение результатов игр из файла
long LoadRecords(long *score, int MapSize, int level){
	std::ifstream fin("data/score.tsn", std::ios::binary);
	for(int i=0; i<30; i++)
		fin.read((char*)&score[i],sizeof(score[i]));	//читаем всё
	fin.close();
	return score[10*MapSize + (level - 1)];	//отправляем рекорд текущей игры
}

//запись настроек в файл
void SaveSettings(CONFIG &conf, int *control){
	std::ofstream fout("data/settings.tsn", std::ios::binary);
	fout.write((char*)&conf, sizeof(conf));	//записываем битовое поле в файл настроек
	for(int i=0; i<4; i++)
		fout.write((char*)&control[i], sizeof(control[i]));	//записываем элементы управления
	fout.close();
}

//получение настроек из файла
CONFIG LoadSettings(int *control){
	CONFIG conf;
	std::ifstream fin("data/settings.tsn", std::ios::binary);
	fin.read((char*)&conf,sizeof(conf));
	for(int i=0; i<4; i++)
		fin.read((char*)&control[i], sizeof(control[i]));
	fin.close();
	return conf;
}

//чтение карты
bool LoadMap(std::string file, int &Size, Map &map){
	int len;
	Coords spawn;
	std::ifstream fin(FOLDER_FILE + file, std::ios::binary);
	if(!fin) return false;
	fin.read((char*)&Size, sizeof(Size));
	fin.read((char*)&spawn, sizeof(spawn));
	fin.read((char*)&len, sizeof(len));
	Coords *arr = new Coords[len];
	for(int i=0; i<len; i++)
		fin.read((char*)&arr[i], sizeof(arr[i]));
	map.BorderCoordsCpy(len, arr, spawn);
	delete [] arr;
	fin.close();
	return true;
	
}
