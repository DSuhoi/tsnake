/*** * * * * * * * * * * * * * * ***
 * game.h                          *
 * Класс игры Game, который        *
 * объединяет другие классы Map,   *
 * Menu и Snake. В нём реализованы *
 * инициализация всех компоентов и *
 * их удаление, обработка состояния*
 * игры и вывод результатов.       *
 *                                 *
 * Created by DSuhoi (2020) (C++11)*
 *** * * * * * * * * * * * * * * ***/


#ifndef __GAME_H__
#define __GAME_H__

#include "../player/snake.h"
#include "../menu/map.h"
#include "../menu/menu.h"
#include <ctime>	// Библиотека для работы со временем

// Флаги состояния игры
enum STATUS_GAME{GAME_WIN = 0,	// Игра окончена
			GAME_NOT_WIN,		// Игра еще идёт
			GAME_RESTART,		// Игра перезапущена
			RETURN_MENU,		// Произошло возвращение в меню без проигрыша игры
			GAME_END };			// Игра завершена


class Game {	// Класс игры
private: 
	bool isGame; 	// Игра начадась
	Menu menu;		// Меню
	Map map;		// Карта игры
	Snake *snake;	// Указатель на объект змеи
	time_t GameTime;	// Время начала игры
	long gameScore[30];		// Игровые рекорды

	void StartGame(int mode);	// Метод настройки поля
	bool CheckWin();	// Проверка на проигрыш
	int GenScore(int level);	// Генерация счёта
	void SelectCustomMap();	// Выбор карты из файла
public:
	~Game(){ EndGame(); }	// Деструктор класса
	void Start();	// Метод инициализации компонентов
	void Process();	// Игровой процесс (основная логика)
	void EndGame();	// Завершение игры
	
};

#endif
