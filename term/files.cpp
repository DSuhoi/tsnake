#include <fstream>
#include <cstring>
#include "files.h"

// Запись результата игры в файл
void FileSystem::SaveRecords(long *score)
{
	std::ofstream fout(FILE_NAME_SCORE_STR, std::ios::binary);
	for(int i = 0; i < 30; i++)	// 3 типа карты * 10 уровней скорости = 30 
		fout.write((char*)&score[i], sizeof(score[i]));	// Пишем всё обратно
	fout.close();
}

// Получение результатов игр из файла
long FileSystem::LoadRecords(long *score, int MapSize, int level)
{
	std::ifstream fin(FILE_NAME_SCORE_STR, std::ios::binary);
	for(int i = 0; i < 30; i++){
		fin.read((char*)&score[i], sizeof(score[i]));	// Читаем всё
	}
	fin.close();
	return score[10*MapSize + (level - 1)];	// Отправляем рекорд текущей игры
}

// Запись настроек в файл
void FileSystem::SaveSettings(CONFIG &conf, int *control)
{
	std::ofstream fout(FILE_NAME_SETTINGS_STR, std::ios::binary);
	fout.write((char*)&conf, sizeof(conf));	// Записываем битовое поле в файл настроек
	for(int i = 0; i < 4; i++){
		fout.write((char*)&control[i], sizeof(control[i]));	// Записываем элементы управления
	}
	fout.close();
}

// Получение настроек из файла
CONFIG FileSystem::LoadSettings(int *control)
{
	CONFIG conf;
	std::ifstream fin(FILE_NAME_SETTINGS_STR, std::ios::binary);
	fin.read((char*)&conf, sizeof(conf));
	for(int i = 0; i < 4; i++){
		fin.read((char*)&control[i], sizeof(control[i]));
	}
	fin.close();
	return conf;
}

// Чтение карты
bool FileSystem::LoadMap(char *fullFileName, int &Size, Map &map)
{	
	std::ifstream fin(fullFileName, std::ios::binary);
	if(!fin){ 
		return false;
	}
	fin.read((char*)&Size, sizeof(Size));
	Coords spawn;
	fin.read((char*)&spawn, sizeof(spawn));
	int len;
	fin.read((char*)&len, sizeof(len));
	Coords *arr = new Coords[len];
	for(int i = 0; i < len; i++){
		fin.read((char*)&arr[i], sizeof(arr[i]));
	}
	map.BorderCoordsCpy(arr, len, spawn);
	delete [] arr;
	fin.close();
	return true;
}
