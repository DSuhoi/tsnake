#include <time.h>
#include "../term/periph.h"
#include "../term/files.h"
#include "game.h"


//метод инициализации компонентов
void Game::Start(){
	initPeriph();
	map = Map();
	snake = NULL;
	Display::InitColor();	//настройка цветов
	menu.InitMainMenu();		//настройка главного меню
	map.InitMap();			//настройка размера карты
	Display::Update();				//обновление экрана
}

//метод настройки поля
void Game::StartGame(int mode){	//настройка:
	if(mode==1) SelectCustomMap();	//если выбрана карта из файла
	map.SelectSizeMap(menu.GetConfigMap().mapSize);	//размер поля
	snake = new Snake();
	snake->InitSnake(map.GetSpawnSnake(), map.GetHeight()*map.GetWidth(), menu.GetConfigMap().teleport);	//размер змеи и способность к телепортации
	if(menu.GetConfigMap().border && !mode)		//если на поле нужны препятствия
	map.InitBorderCoords(snake->InfoHead());		//то создаем их
	map.InitFruitCoords(menu.GetConfigMap().fruitSize);	//кол-во фруктов и их расположение	
	Display::Update();				//обновление экрана
	GameTime = time(0);		//запоминаем время начала игры
	isGame = true;			//игра началась!
}

//проверка на проигрыш
bool Game::CheckWin(){
	Coords tmp = snake->InfoHead();	//положение головы
	//если голова не пересекла границы поля, своё тело или препятствие (если оно есть)
	if(tmp.x == 0 || tmp.x == (map.GetWidth()) || tmp.y == 0 || 
	tmp.y == (map.GetHeight()) || map.IsSnake(snake->InfoHead(),snake->GetBodyCoords(),snake->GetSnakeLen())
	|| map.IsBord(snake->InfoHead())) return GAME_WIN;	//то возвращаем флаг окончания игры
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
	menu.GetConfigMap().mapSize = Size;	//присваиваем переменную
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
	
	if(menu.GetConfigMap().clearScore){	//если нужно стереть данные
	for(int i=0; i<30; i++) gameScore[i] = 0;	//обнуляем и записываем в файл
	SaveRecords(gameScore); menu.GetConfigMap().clearScore = false; 
	menu.GetConfigMap().clearScore = 0;
	} 
	
	long resultLastGame = LoadRecords(gameScore, menu.GetConfigMap().mapSize, menu.GetConfigMap().speed);
	long resultThisGame = 0;
	
	map.PrintSubMenuStatic(resultLastGame, menu.GetConfigMap().speed);
	
	int oldSnakeLen = snake->GetSnakeLen();	//предыдущая длина змеи
	
	do{	//цикл игры
		
		//создание задержки (нужно ещё доработать этот алгоритм)
		cnt = periph(menu.SetControl(), (float)10/menu.GetConfigMap().speed);	//обрабатываем кнопки по пользовательскому шаблону
		
		switch (cnt){
		case 'h': menu.HelpLoop();  break;	//запускаем меню
		case 'p': cnt = menu.PauseLoop(); if(cnt==GAME_END) return; break;	//берём паузу
		case KEY_EXIT: return; break;	//выходим из игры
		case KEY_ENTER:	//если другие клавиши не для управления, то
		case ERR: snake->Move(snake->GetVector()); break;	//перемещаемся без поворотов
		default: snake->Move(cnt); break;	//иначе задаем новый вектор движению игрока
		};
		
		if(map.IsFruit(snake->InfoHead())){	//если змея съела фрукт, то увеличиваем её длину
			snake->IncSnakeLen();
			map.SetFruitOnMap(snake->InfoHead(), snake->GetBodyCoords(), snake->GetSnakeLen());
		}
		
		//если активен режим телепорта
		if(menu.GetConfigMap().teleport){
			if(snake->InfoHead().x == 0) snake->SetHeadCoords(map.GetWidth()-1, snake->InfoHead().y);
			else if(snake->InfoHead().x == map.GetWidth()) snake->SetHeadCoords(1, snake->InfoHead().y);
			else if(snake->InfoHead().y == 0) snake->SetHeadCoords(snake->InfoHead().x, map.GetHeight()-1);
			else if(snake->InfoHead().y == map.GetHeight()) snake->SetHeadCoords(snake->InfoHead().x, 1);
		}
		
		map.UpdateMap(snake->GetBodyCoords(), snake->GetSnakeLen());	//обновляем карту	
		
		if(snake->GetSnakeLen()>oldSnakeLen){
			oldSnakeLen = snake->GetSnakeLen();
			resultThisGame += GenScore(menu.GetConfigMap().speed);//(snake->getSnakeLen()-START_SEG)*12;
		}
		//выводим текущие значения счета игры, уровня скорости и времени
		map.PrintSubMenuActive(resultThisGame, GameTime);
		
		//проверяем игру с учётом выбора в меню паузы
	}while(CheckWin()==GAME_NOT_WIN && cnt!=RETURN_MENU && cnt!=GAME_RESTART);
	
	//убиваем змею
	//snake->KillSnake(map);
	map.SetMap(snake->InfoHead().x, snake->InfoHead().y, KILL);
	
	sleep(50);	//задержка в 500мс
	
	if(resultLastGame<resultThisGame){	//сохраняем результаты игры
		gameScore[menu.GetConfigMap().mapSize*10 + (menu.GetConfigMap().speed-1)] = resultThisGame;
		SaveRecords(gameScore);
	}

	//если не был запланирован выход из игры (проигрыш)
	if(cnt<GAME_RESTART)	//даем пользователю выбор
	if(menu.PrintInfo(true, INFO_WIDTH, INFO_HEIGHT,(char*)"  Restart Game ?"))
		cnt = GAME_RESTART;	//если выбрана перезагрузка карты
	
	delete snake;			//удаляем змею
	snake = NULL;
	
	map.EraseMap();			//удаляем карту
	Display::Update();				//обновляем экран
	
	isGame = false;			//игра окончена		
	
	}
}

//завершение игры
void Game::EndGame(){
	if(snake!=NULL) delete snake;	//удаление объекта игрока
	map.~Map();		//удаление объекта карты
	endPeriph();
}
