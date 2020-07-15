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
#include "../term/periph.h"
#include "../term/files.h"
#include "map.h"

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

const int STRLEN = 18;	//длина строкр


class Menu {
private:
	WINDOW *menu;		//указатель на главное меню
	WINDOW *info;		//указатель на дополнительное меню
	int buttons[4];		//настройки управления
	CONFIG conf;		//настройки карты
	char NameFile[STRLEN];	//файл карты
public:
	void InitMainMenu();	//инициализация главного меню
	int MainMenuLoop(); 	//цикл главного меню
	void SelectCustomMap(Map &map);	//выбор карты из файла
	bool SearchMap();		//поиск карты
	void LvlSettingsLoop();		//меню настроек игры
	void ControlSettingsLoop();	//меню настроек управления
	void HelpLoop();			//информация
	int PauseLoop();			//окно паузы
	bool PrintInfo(bool isSelect, int w, int h, char *buff);	//вывод информации в отдельном окне
	CONFIG& GetConfig();		//возвращение настроек карты
	int* SetControl();		//возвращение настроек управления
};



#endif
