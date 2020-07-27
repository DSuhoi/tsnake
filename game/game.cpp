#include <iostream>
#include "../term/periph.h"
#include "../term/files.h"
#include "game.h"


// Метод инициализации компонентов
void Game::Start()
{
	Periph::InitPeriph();	// Настройка периферии
	map = Map();			// Создание карты
	snake = nullptr;		// Нулевой указатель
	Display::InitColor();	// Настройка цветов
	menu.InitMainMenu();	// Настройка главного меню
	map.InitMap();			// Настройка размера карты
	Display::Update();		// Обновление экрана
}

// Метод настройки поля
void Game::StartGame(int mode)
{	// Настройка:
	if(mode == 1) 
		SelectCustomMap();	// Выбор пользовательской карты
	map.SelectSizeMap(menu.GetConfigMap().mapSize);	// Размер поля
	
	snake = new Snake(); // Создание игрока
	// Размер змеи и её способность к телепортации
	snake->InitSnake(map.GetSpawnSnake(), map.GetHeight()*map.GetWidth());
	if(menu.GetConfigMap().border && !mode)		// Если на поле нужны препятствия, то создаём их
		map.InitBorderCoords(snake->InfoHead());
	map.InitFruitCoords(menu.GetConfigMap().numFruits);	// Кол-во фруктов и их расположение	
	Display::Update();		// Обновление экрана
	GameTime = time(0);		// Запоминаем время начала игры
	isGame = true;			// Игра началась!
}

// Проверка на проигрыш
bool Game::CheckWin()
{
	Coords tmp = snake->InfoHead();	// Положение головы
	// Если голова не пересекла границы поля, своё тело или препятствие (если оно есть)
	if(tmp.x == 0 || tmp.x == (map.GetWidth()) || tmp.y == 0 || 
	tmp.y == (map.GetHeight()) || map.IsSnake(snake->InfoHead(),snake->GetBodyCoords(),snake->GetSnakeLen())
	|| map.IsBorder(snake->InfoHead())){ 
		return GAME_WIN;	// То возвращаем флаг окончания игры
	}
	else{ 
		return GAME_NOT_WIN;	// Иначе игра ещё идёт
	}
}

// Генерация счёта
int Game::GenScore(int level)
{
	srand(time(0));
	return level + rand()%(level + 5);
	
}

void Game::SelectCustomMap()
{
	int Size;
	FileSystem::LoadMap(menu.GetFullFileName(), Size, map);	// Если такого файла нет
	menu.GetConfigMap().mapSize = Size;	// Присваиваем переменную
}


// Игровой процесс (основная логика)
void Game::Process(){
	
	int cnt = 0;		// Переменная для сохранения промежуточных значений результата игры
	int modeMap = 0;	// Режим игры
	
	while(1){
	
		if(cnt != GAME_RESTART){ 	// Если не было дано команды перезагрузить игру, то заходим в меню
			switch(menu.MainMenuLoop()){
			case 0: return; break;	// Если есть команда из меню об окончании, то выходим из игры
			case 1: modeMap = 0; break;	// Если это обычная игра
			case 2: modeMap = 1; break;	// Если это игра на карте из файла
			}; 	
		}
	
		StartGame(modeMap);		// Настройка игры
	
		if(menu.GetConfigMap().clearScore){	// Если нужно стереть данные
			// Обнуляем и записываем в файл
			for(int i = 0; i < 30; i++){
				gameScore[i] = 0;
			}	
			FileSystem::SaveRecords(gameScore); 
			menu.GetConfigMap().clearScore = false; 
			menu.GetConfigMap().clearScore = 0;
		} 
	
		long resultLastGame = FileSystem::LoadRecords(gameScore, menu.GetConfigMap().mapSize, menu.GetConfigMap().speed);
		long resultThisGame = 0;
	
		map.PrintSubMenuStatic(resultLastGame, menu.GetConfigMap().speed);
	
		int oldSnakeLen = snake->GetSnakeLen();	// Предыдущая длина змеи
	
		do{	// Цикл игры
		
			// Создание задержки (нужно ещё доработать этот алгоритм)
			cnt = Periph::GetButton(menu.SetControl(), (float)10/menu.GetConfigMap().speed);	// Обрабатываем кнопки по пользовательскому шаблону
		
			switch (cnt){
			case 'h': menu.HelpLoop();  break;	// Запускаем меню
			case 'p': cnt = menu.PauseLoop(); if(cnt==GAME_END) return; break;	// Берём паузу
			case KEY_EXIT: return; break;	// Выходим из игры
			case KEY_ENTER:	// Если другие клавиши не для управления, то
			case ERR: snake->Move(snake->GetVector()); break;	// Перемещаемся без поворотов
			default: snake->Move(cnt); break;	// Иначе задаем новый вектор движению игрока
			};
		
			// Если активен режим телепорта
			if(menu.GetConfigMap().teleport){
				if(snake->InfoHead().x == 0){ 
					snake->SetHeadCoords(map.GetWidth()-1, snake->InfoHead().y);
				}
				else if(snake->InfoHead().x == map.GetWidth()){
					snake->SetHeadCoords(1, snake->InfoHead().y);
				}
				else if(snake->InfoHead().y == 0){ 
					snake->SetHeadCoords(snake->InfoHead().x, map.GetHeight()-1);
				}
				else if(snake->InfoHead().y == map.GetHeight()){ 
					snake->SetHeadCoords(snake->InfoHead().x, 1);
				}
			}
			
			if(map.IsFruit(snake->InfoHead())){	// Если змея съела фрукт, то увеличиваем её длину
				snake->IncSnakeLen();
				map.SetFruitOnMap(snake->InfoHead(), snake->GetBodyCoords(), snake->GetSnakeLen());
			}
			
			map.UpdateMap(snake->GetBodyCoords(), snake->GetSnakeLen());	// Обновляем карту	
		
			if(snake->GetSnakeLen()>oldSnakeLen){
				oldSnakeLen = snake->GetSnakeLen();
				resultThisGame += GenScore(menu.GetConfigMap().speed);//(snake->getSnakeLen()-START_SEG)*12;
			}
			// Выводим текущие значения счета игры, уровня скорости и времени
			map.PrintSubMenuActive(resultThisGame, GameTime);
		
		// Проверяем игру с учётом выбора в меню паузы
		}while(CheckWin() == GAME_NOT_WIN && cnt != RETURN_MENU && cnt != GAME_RESTART);
	
		// Убиваем змею
		map.SetMap(snake->InfoHead().x, snake->InfoHead().y, KILL);
	
		Periph::GameDelay(20);	// Задержка в 200мс
	
		if(resultLastGame < resultThisGame){	// Сохраняем результаты игры
			gameScore[menu.GetConfigMap().mapSize*10 + (menu.GetConfigMap().speed-1)] = resultThisGame;
			FileSystem::SaveRecords(gameScore);
		}

		// Если не был запланирован выход из игры (проигрыш)
		if(cnt < GAME_RESTART){	// Даем пользователю выбор
			if(menu.PrintInfo(true, INFO_WIDTH, INFO_HEIGHT,(char*)"  Restart Game ?")){
				cnt = GAME_RESTART;	// Если выбрана перезагрузка карты
			}
		}
	
		delete snake;			// Удаляем змею
		snake = NULL;
	
		map.EraseMap();			// Удаляем карту
		Display::Update();		// Обновляем экран
	
		isGame = false;			// Игра окончена		
	
	}
}

// Завершение игры
void Game::EndGame()
{
	if(snake != NULL){ 
		delete snake;	// Удаление объекта игрока
	}
	map.~Map();		// Удаление объекта карты
	Periph::ErasePeriph();
}
