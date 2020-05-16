#include <fstream> 
#include <time.h>
#include "../term/periph.h"
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
void Game::StartGame(){	//настройка:
	map.selectMap(menu.getConfig().mapSize);	//размер поля
	snake = new Snake();
	snake->initSnake(map, menu.getConfig().teleport);	//размер змеи и способность к телепортации
	map.initFruit(menu.getConfig().fruitSize);	//кол-во фруктов и их расположение
	if(menu.getConfig().border)		//если на поле нужны препятствия
	map.initBord(snake->info());		//то создаем их
	halfdelay(10/menu.getConfig().speed);	//создание задержки (нужно ещё доработать этот алгоритм)
	update();				//обновление экрана
	GameTime = time(0);		//запоминаем время начала игры
	isGame = true;			//игра началась!
}

//проверка на проигрыш
bool Game::checkWin(){
	Coords tmp = snake->info();	//положение головы
	//если голова не пересекла границы поля, своё тело или препятствие (если оно есть)
	if(tmp.x == 1 || tmp.x == (map.getWidth()-2) || tmp.y == 1 || 
	tmp.y == (map.getHeight()-1) || map.isSnake(snake->info(),snake->getBody(),snake->getSnakeLen())
	|| (menu.getConfig().border && map.isBord(snake->info()))){
		return GAME_WIN;	//то возвращаем флаг окончания игры
	}
	else return GAME_NOT_WIN;	//иначе игра ещё идёт
}

//игровой процесс (основная логика)
void Game::process(){
	
	int cnt = 0;	//переменная для сохранения промежуточных значений результата игры
	
	Repeat:		//метка goto
	
	if(cnt != GAME_RESTART) //если не было дано команды перезагружаться, то заходим в меню
	if(menu.MainMenuLoop()) return;	//если есть команда из меню об окончании, то выходим из игры
	
	StartGame();	//настройка игры
	
	if(menu.getConfig().clearScore){	//если нужно стереть данные
	for(int i=0; i<30; i++) gameScore[i] = 0;	//обнуляем и записываем в файл
	OutputRecords(gameScore); menu.getConfig().clearScore = false; } 
	
	long resultLastGame = InputRecords(gameScore, menu.getConfig().mapSize, menu.getConfig().speed);
	long resultThisGame = 0;
	
	map.printSubMenuStatic(resultLastGame, menu.getConfig().speed);
	
	do{	//цикл игры
		resultThisGame = (snake->getSnakeLen()-START_SEG)*12;
		
		cnt = periph(menu.setControl());	//обрабатываем кнопки по пользовательскому шаблону
		
		switch (cnt){
		case 'h': menu.HelpLoop(); break;	//запускаем меню
		case 'p': cnt = menu.PauseLoop(); if(cnt==GAME_END) return; break;	//берём паузу
		case KEY_EXIT: return; break;	//выходим из игры
		case KEY_ENTER:	//если другие клавиши не для управления, то
		case ERR: snake->move(map,snake->getVector()); break;	//передвигаемся в предыдущем напрывлении
		default: snake->move(map, cnt); break;	//иначе задаем новый вектор движению игрока
		};
		//выводим текущие значения счета игры, уровня скорости и времени
		map.printSubMenuActive(resultThisGame, GameTime);
		//проверяем игру с учётом выбора в меню паузы
	}while(checkWin()==GAME_NOT_WIN && cnt!=RETURN_MENU && cnt!=GAME_RESTART);
	//убиваем змею
	snake->killSnake(map);
	//если не был запланирован выход из игры (проигрыш)
	if(cnt<GAME_RESTART)	//даем пользователю выбор
	if(menu.PrintInfo(true, InfoWidth, InfoHeight,(char*)"  Restart Game ?"))
		cnt = GAME_RESTART;	//если выбрана перезагрузка карты
		
	if(resultLastGame<resultThisGame){
		gameScore[menu.getConfig().mapSize*10 + (menu.getConfig().speed-1)] = resultThisGame;
		OutputRecords(gameScore);
	}
	
	delete snake;			//удаляем змею
	snake = NULL;
	
	map.endMap();			//удаляем карту
	update();				//обновляем экран
	
	isGame = false;			//игра окончена		
	
	goto Repeat;	//единственная метка goto для упрощения работы цикла

}

//запись результата игры в файл
void Game::OutputRecords(long *score){
	std::ofstream fout("data/score.tsn", std::ios::binary);
	for(int i=0; i<30; i++)	// 3 типа карты * 10 уровней скорости = 30 
	fout.write((char*)&score[i],sizeof(score[i]));	//пишем всё обратно
	fout.close();
}

//получение результатов игр из файла
long Game::InputRecords(long *score, int MapSize, int level){
	std::ifstream fin("data/score.tsn", std::ios::binary);
	for(int i=0; i<30; i++)
	fin.read((char*)&score[i],sizeof(score[i]));	//читаем всё
	fin.close();
	return score[10*MapSize + (level - 1)];	//отправляем рекорд текущей игры
}

//завершение игры
void Game::endGame(){
	if(snake!=NULL) delete snake;	//удаление объекта игрока
	map.~Map();		//удаление объекта карты
	endPeriph();
}
