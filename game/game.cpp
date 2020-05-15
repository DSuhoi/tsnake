#include <time.h>	
#include "../display/display.h"
#include "../periph/periph.h"
#include "../map/map.h"
#include "../menu/menu.h"
#include "../player/snake.h"
#include "game.h"


//метод инициализации компонентов
void Game::Start(){
	initPeriph();
	map = Map();
	snake = Snake();
	Display::init_color();	//настройка цветов
	menu.initMainMenu();		//настройка главного меню
	map.initMap();			//настройка размера карты
	update();				//обновление экрана
}

//метод настройки поля
void Game::StartGame(){	//настройка:
	map.selectMap(menu.getConfig().mapSize);	//размер поля
	snake.initSnake(map, menu.getConfig().teleport);	//размер змеи и способность к телепортации
	map.initFruct(menu.getConfig().fructSize);	//кол-во фруктов и их расположение
	if(menu.getConfig().border)		//если на поле нужны препятствия
	map.initBord(snake.info());		//то создаем их
	halfdelay(10/menu.getConfig().speed);	//создание задержки (нужно ещё доработать этот алгоритм)
	update();				//обновление экрана
	GameTime = time(0);		//запоминаем время начала игры
	isGame = true;			//игра началась!
}

//проверка на проигрыш
bool Game::checkWin(){
	Coords tmp = snake.info();	//положение головы
	//если голова не пересекла границы поля, своё тело или препятствие (если оно есть)
	if(tmp.x == 1 || tmp.x == (map.getWidth()-2) || tmp.y == 1 || 
	tmp.y == (map.getHeight()-1) || map.isSnake(snake.info(),snake.getBody(),snake.getSnakeLen())
	|| (menu.getConfig().border && map.isBord(snake.info()))){
		return GAME_WIN;	//то возвращаем флаг окончания игры
	}
	else return GAME_NOT_WIN;	//иначе игра ещё идёт
}

//игровой процесс (основная логика)
void Game::process(){
	
	int cnt = 0;	//переменная для сохранения промежуточных значений результата игры
	
	Repeat:		//метка goto
	
	if(cnt != GAME_RESTART) //если не было дано команды перезагружаться, то заходим в меню
	if(menu.MainMenuLoop()) endGame();	//если есть команда из меню об окончании, то выходим из игры
	
	StartGame();	//настройка игры
	
	do{	//цикл игры
		cnt = periph(menu.setControl());	//обрабатываем кнопки по пользовательскому шаблону
		
		switch (cnt){
		case 'h': menu.HelpLoop(); break;	//запускаем меню
		case 'p': cnt = menu.PauseLoop(); if(cnt==GAME_END) endPeriph(); break;	//берём паузу
		case KEY_EXIT: endGame(); break;	//выходим из игры
		case KEY_ENTER:	//если другие клавиши не для управления, то
		case ERR: snake.move(map,snake.getVector()); break;	//передвигаемся в предыдущем напрывлении
		default: snake.move(map, cnt); break;	//иначе задаем новый вектор движению игрока
		};
		//выводим текущие значения счета игры, уровня скорости и времени
		map.printSubMenu((snake.getSnakeLen()-START_SEG)*12,menu.getConfig().speed, GameTime);
		//проверяем игру с учётом выбора в меню паузы
	}while(checkWin()==GAME_NOT_WIN && cnt!=RETURN_MENU && cnt!=GAME_RESTART);
	//убиваем змею
	snake.killSnake(map);
	//если не был запланирован выход из игры (проигрыш)
	if(cnt<GAME_RESTART)	//даем пользователю выбор
	if(menu.PrintInfo(true, InfoWidth, InfoHeight,(char*)"  Restart Game ?"))
		cnt = GAME_RESTART;	//если выбрана перезагрузка карты
	
	snake.endSnake();		//удаляем змею
	map.endMap();			//удаляем карту
	update();				//обновляем экран
	
	isGame = false;			//игра окончена		
	
	goto Repeat;	//единственная метка goto для упрощения работы цикла

}

//завершение игры
void Game::endGame(){
	snake.~Snake();	//удаление объекта игрока
	map.~Map();		//удаление объекта карты
	endPeriph();
}
