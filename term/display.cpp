#include "display.h"


// Метод обёртка
void Display::Update()
{
	refresh();	// Функция обновления экрана
}

// Обновление окна
void Display::Update(WINDOW *win)
{
	wclear(win);	// Очищаем окно
	wrefresh(win);	// Обновляем окно
	box(win, 0, 0);	// Устанавливаем границы окна
	refresh();	// Обновление
}


// Удаление окна
void Display::DeleteWindow(WINDOW *win)
{
	if(win!=nullptr){	// Если указатель на окно не пустой
		 wclear(win);	// Очистка окна
		 wrefresh(win); // Обновление окна
		 delwin(win); 	// Удаление окна
	} 
	win = nullptr; 
}

// Инициализация основных цветов
void Display::InitColor()
{	// Присвоение цветов
	init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
	init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(RED, COLOR_RED, COLOR_BLACK);
	init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
}

// Вывод текста
void Display::PrintScr(WINDOW *win, int x, int y, char *buff)
{
	mvwprintw(win, y, x, "%s", buff);	// Отсылаем массив по координатам
	wrefresh(win);	// и обновляем окно
}

// Вывод цветного текста
void Display::PrintScr(WINDOW *win, int x, int y, char *buff, int color)
{
	wattron(win,COLOR_PAIR(color));	// Задаем цвет
	PrintScr(win, x, y, buff);
	wattroff(win,COLOR_PAIR(color));// Отключаем цвет
	wrefresh(win);		// и обновляем окно
}

// Вывод символа
void Display::PrintScr(WINDOW *win, int x, int y, chtype ch)
{
	wmove(win,y,x);	// Ставим курсор на позицию
	wdelch(win);	// Удаляем символ
	winsch(win,ch);	// Вставляем свой символ
	wrefresh(win);	// Обновляем окно
}
