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
#include <string>


const std::string FOLDER_FILE = "data/maps/";


struct CONFIG {		//битовое поле с настройками карты
unsigned speed: 4;		//скорость игрока [1..10]
unsigned mapSize: 2;	//размер карты [0..2]
unsigned fruitSize: 7;	//кол-во фруктов [1..99]
unsigned border: 1;		//препятствия 1/0
unsigned teleport: 1;	//телепортация змеи 1/0
unsigned clearScore: 1;	//очистка рекордов 1/0
};

void SaveRecords(long *score);	//запись результата игры в файл
long LoadRecords(long *score, int MapSize, int level);	//получение результатов игр из файла

void SaveSettings(CONFIG &conf, int *control);	//запись настроек в файл
CONFIG LoadSettings(int *control);				//получение настроек из файла

bool LoadMap(std::string file, int &Size, Map &map);	//чтение карты

#endif
