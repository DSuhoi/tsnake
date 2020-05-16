/*** * * * * * * * * * * * * * * * * * * * * * * * * * ***
 * menu.h                                                *
 * Класс Menu содержит в себе указатели на окна, а также *
 * настройки управления и карты. В классе находятся      *
 * методы, отвечающих за меню настроек и информации.     *
 * По мере развития игры этот файл будет дополняться     *
 * новыми элементами.                                    *
 *                                                       *
 * Created by DSuhoi (2020) (C++11)                      *
 *** * * * * * * * * * * * * * * * * * * * * * * * * * ***/

#ifndef __MENU_H__
#define __MENU_H__

#include <ncurses.h>
#include "../periph/periph.h"

//Размеры:
//главного меню
const int MenuHeight = 12;
const int MenuWidth = 20;

//меню помощи
const int HelpHeight = 10;
const int HelpWidth = 35;
//сдвиг в этом меню для постановки элементов на правой стороне
const int HelpWidthRight = 22;

//меню паузы
const int PauseWidth = 18;
const int PauseHeight = 6;

//информационное меню
const int InfoHeight = 4;
const int InfoWidth = 20;


struct CONFIG {		//битовое поле с настройками карты
unsigned speed: 5;		//скорость игрока [1..10]
unsigned mapSize: 2;	//размер карты [0..2]
unsigned fruitSize: 7;	//кол-во фруктов [1..99]
unsigned border: 1;		//препятствия 1/0
unsigned teleport: 1;	//телепортация змеи 1/0
};


class Menu {
private:
	WINDOW *menu;		//указатель на главное меню
	WINDOW *info;		//указатель на дополнительное меню
	CONTROL buttons;	//настройки управления
	CONFIG conf;		//настройки карты
public:
	void initMainMenu();	//инициализация главного меню
	bool MainMenuLoop(); 	//цикл главного меню
	void LvlSettingsLoop();		//меню настроек игры
	void ControlSettingsLoop();	//меню настроек управления
	void HelpLoop();			//информация
	int PauseLoop();			//окно паузы
	bool PrintInfo(bool isSelect, int w, int h, char *buff);	//вывод информации в отдельном окне
	CONFIG& getConfig();		//возвращение настроек карты
	CONTROL& setControl();		//возвращение настроек управления
};



#endif
