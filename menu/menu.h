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

//#include <ncurses.h>
#include "../term/periph.h"
#include "../term/files.h"

//Размеры:

//главного меню
const int MENU_HEIGHT = 12;
const int MENU_WIDTH = 20;

//меню помощи
const int HELP_HEIGHT = 10;
const int HELP_WIDTH = 35;
//сдвиг в этом меню для постановки элементов на правой стороне
const int Help_WIDTH_RIGHT = 22;

//меню паузы
const int PAUSE_HEIGHT = 6;
const int PAUSE_WIDTH = 18;

//информационное меню
const int INFO_HEIGHT = 4;
const int INFO_WIDTH = 20;

const int STR_LEN = 18;	//длина строкр


class Menu {
private:
	WINDOW *menuWidow;		//указатель на главное меню
	WINDOW *infoWidow;		//указатель на дополнительное меню
	int buttons[4];		//настройки управления
	CONFIG configMap;		//настройки карты
	char nameFile[STR_LEN];	//файл карты
protected:
	bool SearchMap();		//поиск карты
	void LvlSettingsLoop();		//меню настроек игры
	void ControlSettingsLoop();	//меню настроек управления
public:
	void InitMainMenu();	//инициализация главного меню
	int MainMenuLoop(); 	//цикл главного меню
	void HelpLoop();		//информация
	int PauseLoop();		//окно паузы
	char* GetNameFile();	//возвращение названия файла
	bool PrintInfo(bool Select, int width, int height, char *stringWithInfo);	//вывод информации в отдельном окне
	CONFIG& GetConfigMap();		//возвращение настроек карты
	int* SetControl();		//возвращение настроек управления
};

#endif
