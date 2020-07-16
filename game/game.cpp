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
	menu.InitMainMenu();		//настройка главного меню
	map.InitMap();			//настройка размера карты
	update();				//обновление экрана
}

//метод настройки поля
void Game::StartGame(int mode){	//настройка:
	if(mode==1) SelectCustomMap();	//если выбрана карта из файла
	map.SelectMap(menu.GetConfig().mapSize);	//размер поля
	snake = new Snake();
	snake->InitSnake(map.GetSpawnSnake(), map.GetHeight()*map.GetWidth(), menu.GetConfig().teleport);	//размер змеи и способность к телепортации
	if(menu.GetConfig().border && !mode)		//если на поле нужны препятствия
	map.InitBord(snake->Info());		//то создаем их
	map.InitFruit(menu.GetConfig().fruitSize);	//кол-во фруктов и их расположение	
	update();				//обновление экрана
	GameTime = time(0);		//запоминаем время начала игры
	isGame = true;			//игра началась!
}

//проверка на проигрыш
bool Game::CheckWin(){
	Coords tmp = snake->Info();	//положение головы
	//если голова не пересекла границы поля, своё тело или препятствие (если оно есть)
	if(tmp.x == 0 || tmp.x == (map.GetWidth()) || tmp.y == 0 || 
	tmp.y == (map.GetHeight()) || map.IsSnake(snake->Info(),snake->GetBody(),snake->GetSnakeLen())
	|| map.IsBord(snake->Info())) return GAME_WIN;	//то возвращаем флаг окончания игры
	else return GAME_NOT_WIN;	//иначе игра ещё идёт
}

//генерация счёта
int Game::GenScore(int level){
	srand(time(0));
	return level + rand()%(level + 5);
	
}

void Game::SelectCustomMap(){
	int Size;
	LoadMap(menu.GetNameFile(), Size, map);	//если такого файла нет
	menu.GetConfig().mapSize = Size;	//присваиваем переменную
}


//игровой процесс (основная логика)
void Game::Process(){
	
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
	
	if(menu.GetConfig().clearScore){	//если нужно стереть данные
	for(int i=0; i<30; i++) gameScore[i] = 0;	//обнуляем и записываем в файл
	SaveRecords(gameScore); menu.GetConfig().clearScore = false; 
	menu.GetConfig().clearScore = 0;
	} 
	
	long resultLastGame = LoadRecords(gameScore, menu.GetConfig().mapSize, menu.GetConfig().speed);
	long resultThisGame = 0;
	
	map.PrintSubMenuStatic(resultLastGame, menu.GetConfig().speed);
	
	int oldSnakeLen = snake->GetSnakeLen();	//предыдущая длина змеи
	
	do{	//цикл игры
		
		//создание задержки (нужно ещё доработать этот алгоритм)
		cnt = periph(menu.SetControl(), (float)10/menu.GetConfig().speed);	//обрабатываем кнопки по пользовательскому шаблону
		
		switch (cnt){
		case 'h': menu.HelpLoop();  break;	//запускаем меню
		case 'p': cnt = menu.PauseLoop(); if(cnt==GAME_END) return; break;	//берём паузу
		case KEY_EXIT: return; break;	//выходим из игры
		case KEY_ENTER:	//если другие клавиши не для управления, то
		case ERR: snake->Move(snake->GetVector()); break;	//перемещаемся без поворотов
		default: snake->Move(cnt); break;	//иначе задаем новый вектор движению игрока
		};
		
		if(map.IsFruit(snake->Info())){	//если змея съела фрукт, то увеличиваем её длину
			snake->IncSnakeLen();
			map.SetFruitOnMap(snake->Info(), snake->GetBody(), snake->GetSnakeLen());
		}
		
			//если активен режим телепорта
		if(menu.GetConfig().teleport){
			if(snake->Info().x == 0) snake->SetHead(map.GetWidth()-1, snake->Info().y);
			else if(snake->Info().x == map.GetWidth()) snake->SetHead(1, snake->Info().y);
			else if(snake->Info().y == 0) snake->SetHead(snake->Info().x, map.GetHeight()-1);
			else if(snake->Info().y == map.GetHeight()) snake->SetHead(snake->Info().x, 1);
		}
			
		map.UpdateMap(snake->GetBody(), snake->GetSnakeLen());	//обновляем карту	
		
		
		if(snake->GetSnakeLen()>oldSnakeLen){
			oldSnakeLen = snake->GetSnakeLen();
			resultThisGame += GenScore(menu.GetConfig().speed);//(snake->getSnakeLen()-START_SEG)*12;
		}
		//выводим текущие значения счета игры, уровня скорости и времени
		map.PrintSubMenuActive(resultThisGame, GameTime);
		
		
		
		//проверяем игру с учётом выбора в меню паузы
	}while(CheckWin()==GAME_NOT_WIN && cnt!=RETURN_MENU && cnt!=GAME_RESTART);
	
	//убиваем змею
	//snake->KillSnake(map);
	map.SetMap(snake->Info().x, snake->Info().y, KILL);
	
	sleep(50);	//задержка в 500мс
	
	if(resultLastGame<resultThisGame){	//сохраняем результаты игры
		gameScore[menu.GetConfig().mapSize*10 + (menu.GetConfig().speed-1)] = resultThisGame;
		SaveRecords(gameScore);
	}

	//если не был запланирован выход из игры (проигрыш)
	if(cnt<GAME_RESTART)	//даем пользователю выбор
	if(menu.PrintInfo(true, InfoWidth, InfoHeight,(char*)"  Restart Game ?"))
		cnt = GAME_RESTART;	//если выбрана перезагрузка карты
	
	delete snake;			//удаляем змею
	snake = NULL;
	
	map.EndMap();			//удаляем карту
	update();				//обновляем экран
	
	isGame = false;			//игра окончена		
	
	}
}

//завершение игры
void Game::EndGame(){
	if(snake!=NULL) delete snake;	//удаление объекта игрока
	map.~Map();		//удаление объекта карты
	endPeriph();
}
