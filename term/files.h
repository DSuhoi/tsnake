/*** * * * * * * * * * * * * * * * * * * * ***
 * files.h                                   *
 * Библиотека для работы с файловой системой *
 * (запись и чтение данных из файлов).       *
 *                                           *
 * Created by DSuhoi (2020) (C++11)          *
 *** * * * * * * * * * * * * * * * * * * * ***/


#ifndef __FILES_H__
#define __FILES_H__

#include "../menu/map.h"

const char FILE_NAME_SCORE_STR [] = "data/score.tsn";	// Путь до файла с таблицей рекордов
const char FILE_NAME_SETTINGS_STR [] = "data/settings.tsn";	// Путь до файла с настройками
const char FOLDER_MAP_STR [] = "data/maps/";	// Путь до директории с картами
const char FORMAT_MAP_STR [] = ".lvl";			// Разрешение файла карты
const int FULL_FILE_NAME_LEN = 32;				// Длина названия карты

struct CONFIG {			// Битовое поле с настройками карты
unsigned speed: 4;		// Скорость игрока [1..10]
unsigned mapSize: 2;	// Размер карты [0..2]
unsigned numFruits: 7;	// Кол-во фруктов [1..99]
unsigned border: 1;		// Препятствия 1/0
unsigned teleport: 1;	// Телепортация змеи 1/0
unsigned clearScore: 1;	// Очистка рекордов 1/0
};


class FileSystem{
private:
	FileSystem(){}
public:
	static void SaveRecords(long *score);	// Запись результата игры в файл
	static void SaveSettings(CONFIG &conf, int *control);	// Запись настроек в файл
	
	static long LoadRecords(long *score, int MapSize, int level);	// Получение результатов игр из файла
	static CONFIG LoadSettings(int *control);						// Получение настроек из файла
	static bool LoadMap(char* fullFileName, int &Size, Map &map);	// Чтение карты
};

#endif
