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
#include <time.h>	//библиотека для работы со временем

//флаги состояния игры
enum STATUS_GAME{GAME_WIN = 0,	//игра окончена
			GAME_NOT_WIN,	//игра еще идёт
			GAME_RESTART,	//игра перезапущена
			RETURN_MENU,	//произошло возвращение в меню без проигрыша игры
			GAME_END };		//игра завершена


class Game {	//класс игры
private: 
	bool isGame; //игра начадась
	Menu menu;		//меню
	Map map;		//карта игры
	Snake *snake;	//объект змеи
	time_t GameTime;	//время начала игры
	long gameScore[30];		//игровые рекорды
protected:
	void StartGame(int mode);	//метод настройки поля
	bool checkWin();	//проверка на проигрыш
public:
	~Game(){ endGame(); }	//деструктор класса
	void Start();	//метод инициализации компонентов
	void process();	//игровой процесс (основная логика)
	void endGame();	//завершение игры
	
};

#endif
