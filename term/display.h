/*** * * * * * * * * * * * * * * * * * * ***
 * display.h                               *
 * Эта библиотека нужна для упрощения      *
 * взаимодействия между компонентами игры  *
 * и экраном терминала. Функции update и   *
 * deleteWindow часто используются         *
 * для обновления, удаления и очистки окон.*
 * Для переноса программы на Windows или   *
 * Mac нужно редактировать этот файл.      *
 * Created by DSuhoi (2020) (C file)       *
 *** * * * * * * * * * * * * * * * * * * ***/


#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <ncurses.h>

// Перечисления для изменения цвета символа
enum COLORS {GREEN = 1, YELLOW, RED, BLUE };

class Display {
public:
	static void Update(WINDOW *win);	// Обновление окна
	static void Update();				// Обновление экрана в целом
	static void DeleteWindow(WINDOW *win);	// Удаление окна
	
	static void InitColor();	// Инициализация основных цветов
	static void PrintScr(WINDOW *win, int x, int y, chtype ch);	// Вывод символа
	static void PrintScr(WINDOW *win, int x, int y, char *buff);	// Вывод текста
	static void PrintScr(WINDOW *win, int x, int y, char *buff, int color);	// Вывод цветного текста	
	
};

#endif
