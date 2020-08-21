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


#include "../term/periph.h"
#include "../term/files.h"

// Размеры:

// Главного меню
const int MENU_HEIGHT = 12;
const int MENU_WIDTH = 20;

// Меню помощи
const int HELP_HEIGHT = 10;
const int HELP_WIDTH = 35;
// Сдвиг в этом меню для постановки элементов на правой стороне
const int Help_WIDTH_RIGHT = 22;

// Меню паузы
const int PAUSE_HEIGHT = 6;
const int PAUSE_WIDTH = 18;

// Информационное меню
const int INFO_HEIGHT = 4;
const int INFO_WIDTH = 20;


class Menu {
private:
	WINDOW *menuWidow;		// Указатель на главное меню
	WINDOW *infoWidow;		// Указатель на дополнительное меню
	int buttons[4];			// Настройки управления
	CONFIG configMap;		// Настройки карты
	char fullFileName[FULL_FILE_NAME_LEN];	// Полное название файла карты

	void LvlSettingsLoop();		// Меню настроек игры
	void ControlSettingsLoop();	// Меню настроек управления
public:
	void InitMainMenu();	// Инициализация главного меню
	int MainMenuLoop(); 	// Цикл главного меню
	void HelpLoop();		// Информация
	int PauseLoop();		// Окно паузы
	bool PrintInfo(int width, int height, char *stringWithInfo, bool select);	// Вывод информации в отдельном окне
	void PrintError();			// Вывод ошибки в отдельном окне
	CONFIG& GetConfigMap();		// Возвращение настроек карты
	int *GetControl();			// Возвращение настроек управления
	char *GetFullFileName();	// Возвращение полного названия файла карты
};

#endif
