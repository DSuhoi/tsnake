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
void Game::StartGame()
{	// Настройка:
	map.SelectSizeMap(menu.GetConfigMap().mapSize);	// Размер поля
	
	snake = new Snake(); // Создание игрока
	// Размер змеи
	snake->InitSnake(map.GetSpawnSnake(), map.GetHeight() * map.GetWidth());
	if(menu.GetConfigMap().border)		// Если на поле нужны препятствия, то создаём их
		map.InitBorderCoords(snake->InfoHead());
	map.InitFruitCoords(menu.GetConfigMap().numFruits);	// Кол-во фруктов и их расположение	
	Display::Update();		// Обновление экрана
	GameTime = time(0);		// Запоминаем время начала игры
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

// Игровой процесс (основная логика)
void Game::Process()
{
	// Переменная для сохранения промежуточных значений результата игры
	int gameResult = 0;
	while(1){
		// Если не было дано команды перезагрузить игру, то заходим в меню
		if(gameResult != GAME_RESTART && menu.MainMenuLoop() == 0){
			// Если есть команда из меню об окончании, то выходим из игры
			return;
		}
		// Обнуление результата прошлой игры
		gameResult = 0;
		
		// Настройка игры
		StartGame();
	
		if(menu.GetConfigMap().clearScore){	// Если нужно стереть данные
			// Обнуляем и записываем в файл
			for(int i = 0; i < 30; i++){
				gameScore[i] = 0;
			}	
			FileSystem::SaveRecords(gameScore); 
			menu.GetConfigMap().clearScore = false; 
		}
		
		// Чтение результатов прошлых игр
		long resultLastGame = FileSystem::LoadRecords(gameScore, menu.GetConfigMap().mapSize, menu.GetConfigMap().speed);
		// Результат текущей игры
		long resultThisGame = 0;
		
		// Вывод статичной части меню
		map.PrintSubMenuStatic(resultLastGame, menu.GetConfigMap().speed);
		
		int oldSnakeLen = snake->GetSnakeLen();	// Предыдущая длина змеи
		// Цикл игры
		do{
			// Обрабатываем кнопки по пользовательскому шаблону
			int currentButton = Periph::GetButton(menu.GetControl(), static_cast<float>(10/menu.GetConfigMap().speed));
		
			switch (currentButton){
			case 'h':	// Запускаем меню
				menu.HelpLoop();
				break;	
			case 'p':	// Берём паузу
				gameResult = menu.PauseLoop(); 
				if(gameResult == GAME_END){
					return;
				}
				break;
			case KEY_EXIT:	// Выходим из игры
				return; 
				break;	
			case KEY_ENTER:	// Если другие клавиши не для управления, то
			case ERR:	// Перемещаемся без поворотов
				snake->Move(snake->GetVector()); 
				break;	
			default:	// Иначе задаем новый вектор движению игрока
				snake->Move(currentButton); 
				break;
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
		}while(CheckWin() == GAME_NOT_WIN && gameResult != RETURN_MENU && gameResult != GAME_RESTART);
	
		// Убиваем змею
		map.SetMap(snake->InfoHead().x, snake->InfoHead().y, KILL);
	
		Periph::GameDelay(20);	// Задержка в 200мс
	
		if(resultLastGame < resultThisGame){	// Сохраняем результаты игры
			gameScore[menu.GetConfigMap().mapSize*10 + (menu.GetConfigMap().speed-1)] = resultThisGame;
			FileSystem::SaveRecords(gameScore);
		}

		// Если не был запланирован выход из игры (проигрыш)
		if(gameResult < GAME_RESTART){	// Даем пользователю выбор
			if(menu.PrintInfo(INFO_WIDTH, INFO_HEIGHT,(char*)"  Restart Game ?", true)){
				gameResult = GAME_RESTART;	// Если выбрана перезагрузка карты
			}
		}
	
		delete snake;			// Удаляем змею
		snake = nullptr;
	
		map.EraseMap();			// Удаляем карту
		Display::Update();		// Обновляем экран
	}
}

// Завершение игры
void Game::EndGame()
{
	if(snake != nullptr){ 
		delete snake;	// Удаление объекта игрока
	}
	map.~Map();		// Удаление объекта карты
	Periph::ErasePeriph();
}
