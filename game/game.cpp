#include <time.h>
#include "../term/periph.h"
#include "../term/files.h"
#include "game.h"


//метод инициализации компонентов
void Game::Start(){
	initPeriph();
	map = Map();
	snake = NULL;
	init_color();	//настройка цветов
	menu.initMainMenu();		//настройка главного меню
	map.initMap();			//настройка размера карты
	update();				//обновление экрана
}

//метод настройки поля
void Game::StartGame(int mode){	//настройка:
	if(mode==1) menu.SelectCustomMap(map);	//если выбрана карта из файла
	map.selectMap(menu.getConfig().mapSize);	//размер поля
	snake = new Snake();
	snake->initSnake(map, menu.getConfig().teleport);	//размер змеи и способность к телепортации
	if(menu.getConfig().border && !mode)		//если на поле нужны препятствия
	map.initBord(snake->info());		//то создаем их
	map.initFruit(menu.getConfig().fruitSize);	//кол-во фруктов и их расположение	
	update();				//обновление экрана
	GameTime = time(0);		//запоминаем время начала игры
	isGame = true;			//игра началась!
}

//проверка на проигрыш
bool Game::checkWin(){
	Coords tmp = snake->info();	//положение головы
	//если голова не пересекла границы поля, своё тело или препятствие (если оно есть)
	if(tmp.x == 0 || tmp.x == (map.getWidth()) || tmp.y == 0 || 
	tmp.y == (map.getHeight()) || map.isSnake(snake->info(),snake->getBody(),snake->getSnakeLen())
	|| map.isBord(snake->info())) return GAME_WIN;	//то возвращаем флаг окончания игры
	else return GAME_NOT_WIN;	//иначе игра ещё идёт
}

//генерация счёта
int Game::genScore(int level){
	srand(time(0));
	return level + rand()%(level + 5);
	
}

//игровой процесс (основная логика)
void Game::process(){
	
	int cnt = 0;	//переменная для сохранения промежуточных значений результата игры
	int modeMap = 0;	//режим игры
	
	while(1){
	
	if(cnt != GAME_RESTART){ //если не было дано команды перезагрузить игру, то заходим в меню
		switch(menu.MainMenuLoop()){
		case 0: return; break;	//если есть команда из меню об окончании, то выходим из игры
		case 1: modeMap = 0; break;	//если это обычная игра
		case 2: modeMap = 1; break;	//если это игра на карте из файла
		}; 	
	}
	
	StartGame(modeMap);	//настройка игры
	
	if(menu.getConfig().clearScore){	//если нужно стереть данные
	for(int i=0; i<30; i++) gameScore[i] = 0;	//обнуляем и записываем в файл
	SaveRecords(gameScore); menu.getConfig().clearScore = false; 
	menu.getConfig().clearScore = 0;
	} 
	
	long resultLastGame = LoadRecords(gameScore, menu.getConfig().mapSize, menu.getConfig().speed);
	long resultThisGame = 0;
	
	map.printSubMenuStatic(resultLastGame, menu.getConfig().speed);
	
	map.updateMap();	//обновляем карту
	
	int oldSnakeLen = snake->getSnakeLen();	//предыдущая длина змеи
	
	do{	//цикл игры
		//создание задержки (нужно ещё доработать этот алгоритм)
		cnt = periph(menu.setControl(), (float)10/menu.getConfig().speed);	//обрабатываем кнопки по пользовательскому шаблону
		
		switch (cnt){
		case 'h': menu.HelpLoop(); map.updateMap(); break;	//запускаем меню
		case 'p': cnt = menu.PauseLoop(); if(cnt==GAME_END) return; map.updateMap(); break;	//берём паузу
		case KEY_EXIT: return; break;	//выходим из игры
		case KEY_ENTER:	//если другие клавиши не для управления, то
		case ERR: snake->move(map,snake->getVector()); break;	//перемещаемся без поворотов
		default: snake->move(map, cnt); break;	//иначе задаем новый вектор движению игрока
		};
		
		if(snake->getSnakeLen()>oldSnakeLen){
			oldSnakeLen = snake->getSnakeLen();
			resultThisGame += genScore(menu.getConfig().speed);//(snake->getSnakeLen()-START_SEG)*12;
		}
		//выводим текущие значения счета игры, уровня скорости и времени
		map.printSubMenuActive(resultThisGame, GameTime);
		//проверяем игру с учётом выбора в меню паузы
	}while(checkWin()==GAME_NOT_WIN && cnt!=RETURN_MENU && cnt!=GAME_RESTART);
	
	//убиваем змею
	snake->killSnake(map);
	
	sleep(50);	//задержка в 500мс
	
	if(resultLastGame<resultThisGame){	//сохраняем результаты игры
		gameScore[menu.getConfig().mapSize*10 + (menu.getConfig().speed-1)] = resultThisGame;
		SaveRecords(gameScore);
	}

	//если не был запланирован выход из игры (проигрыш)
	if(cnt<GAME_RESTART)	//даем пользователю выбор
	if(menu.PrintInfo(true, InfoWidth, InfoHeight,(char*)"  Restart Game ?"))
		cnt = GAME_RESTART;	//если выбрана перезагрузка карты
	
	delete snake;			//удаляем змею
	snake = NULL;
	
	map.endMap();			//удаляем карту
	update();				//обновляем экран
	
	isGame = false;			//игра окончена		
	
	}
}

//завершение игры
void Game::endGame(){
	if(snake!=NULL) delete snake;	//удаление объекта игрока
	map.~Map();		//удаление объекта карты
	endPeriph();
}
