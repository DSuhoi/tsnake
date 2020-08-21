#include <iostream>
#include <fstream>
#include <cstring>
#include "../term/display.h"
#include "menu.h"


// Инициализация главного меню
void Menu::InitMainMenu()
{
	int screenWidth = 0, screenHeight = 0;	// Размеры экран X*Y
	getmaxyx(stdscr, screenHeight, screenWidth);
	// Создание окна главного меню
	menuWidow = newwin(MENU_HEIGHT, MENU_WIDTH,(screenHeight - MENU_HEIGHT)/2, (screenWidth - MENU_WIDTH)/2);
	// Настройки управления и карты
	buttons[DOWN] = KEY_DOWN;
	buttons[UP] = KEY_UP;
	buttons[LEFT] = KEY_LEFT;
	buttons[RIGHT] = KEY_RIGHT;
	
	configMap = FileSystem::LoadSettings(buttons);
}

// Цикл главного меню
int Menu::MainMenuLoop()
{
	// Массив из названий элементов меню
	std::string menuPart[5] ={"Start Game", "Lvl Settings", "Controls", "Help", "Exit"};
	
	// Очищаем и обновляем окно
	Display::Update(menuWidow);
	// Выбор элемента меню (подсветка выбранного элемента)
	int hiLight = 0;	
	while(1){
		// Вывод названия игры
		Display::PrintScr(menuWidow, MENU_WIDTH/2 - 5, 0, (char*)"TSNAKE 1.3", BLUE);
	
		// Зелёные элементы
		for(int cursPosition = 0; cursPosition < 5; cursPosition++){
			if(cursPosition == hiLight){	// Выбранную строку подсвечиваем
				wattron(menuWidow, COLOR_PAIR(GREEN));
			}
			// Выводим строки
			Display::PrintScr(menuWidow, 2, cursPosition + 1, (char*)menuPart[cursPosition].c_str());
			wattroff(menuWidow, COLOR_PAIR(GREEN));
		}
		// Читаем нажатые клавиши
		switch(Periph::GetButton()){
		case KEY_UP: 
			if(hiLight > 0){	// Выбор элемента меню
				hiLight--;
			} 
			break;	
		case KEY_DOWN: 
			if(hiLight < 4){ 
				hiLight++; 
			}
			break;
		case KEY_EXIT: 
			return 0; 		// Выход из программы
			break;		
		case KEY_ENTER:
			switch (hiLight){
			case 0: 
				return 1; 	// Если это игра, то выходим из меню
				break;	
			case 1: 
				LvlSettingsLoop();  // Настройки уровня
				break;		
			case 2: 
				ControlSettingsLoop(); // Настройки управления
				break;	
			case 3: 
				HelpLoop(); // Информация
				break;				
			case 4: 
				return 0; // Выход из программы
				break;
			}; 
			Display::Update(menuWidow); break;
		};
	}
}

// Меню настроек игры
void Menu::LvlSettingsLoop()
{
	// Элементы меню, параметры карты и ответы
	std::string menuPart[8] = {"Back", "Speed:", "Fruit:","Map Size:", 
		"Border:", "Teleport:", "Erase Settings", "Erase Records"};
	std::string mapSize[3] = {"Small", "Medium", "Large"};
	std::string selectStr[2] = {"Off", "On"};
	// Символьный массив под вывод строк меню
	char buffStr[10];
	// Обновление окна меню
	Display::Update(menuWidow);
	// Вид скобок (0 - прямоугольные, 1 -  острые)
	int currentBracket = 0;
	int hiLight = 0;
	while(1){
	
		Display::PrintScr(menuWidow, MENU_WIDTH/2 - 6, 0, (char*)"Lvl Settings", BLUE);
		
		for(int cursPosition = 0; cursPosition < 8;cursPosition++){
			if(cursPosition == hiLight){
				// Включение зелёной подсветки у выбранного элемента
				wattron(menuWidow, COLOR_PAIR(GREEN));
				currentBracket = 1;
			}
			else{ 
				currentBracket = 0;
			}
			
			// Определение параметра по позиции курсора отрисовки меню
			char parametr[7] = "";
			switch(cursPosition){
			case 1:
				sprintf(parametr, "%d", configMap.speed);
				break;
			case 2:
				sprintf(parametr, "%d", configMap.numFruits);
				break;
			case 3:
				strcpy(parametr, (char*)mapSize[configMap.mapSize].c_str());
				break;
			case 4:
				strcpy(parametr, (char*)selectStr[configMap.border].c_str());
				break;
			case 5:
				strcpy(parametr, (char*)selectStr[configMap.teleport].c_str());
				break;
			};
			
			if(cursPosition == 0){
				// Вывод параметра выхода из меню
				Display::PrintScr(menuWidow, 2, 1, (char*)menuPart[0].c_str());
			}
			else if(cursPosition > 5){
				// Вывод параметра очистки настроек
				Display::PrintScr(menuWidow, 2, (MENU_HEIGHT - 9) + cursPosition, (char*)menuPart[cursPosition].c_str());
			}
			else{
				// Левые и правые скобки
				char bracketL[2] = {'[', '<'};
				char bracketR[2] = {']', '>'};
				// Генерация строки меню
				sprintf(buffStr,"%c%s%c", bracketL[currentBracket], parametr, bracketR[currentBracket]);
				// Вывод названия параметра и его значения (по центру)
				Display::PrintScr(menuWidow, MENU_WIDTH - strlen(buffStr)/2 - 5, cursPosition + 2, buffStr);
				Display::PrintScr(menuWidow, 2, cursPosition + 2, (char*)menuPart[cursPosition].c_str());
			}
		
			// Очистка массива
			for(int index = 0; index < 9; index++){ 
				buffStr[index] = 0;
			}
			// Отключение зелёной подсветки
			wattroff(menuWidow, COLOR_PAIR(GREEN));
		}
		// Обработка нажатых клавиш
		switch(Periph::GetButton()){
		case KEY_UP: 
			if(hiLight > 0){ 
				hiLight--;
			} 
			break;
		case KEY_DOWN: 
			if(hiLight < 7){ 
				hiLight++;
			} 
			break;
		case KEY_LEFT: // В зависимости от элемента настраиваем параметры
			switch(hiLight){
			case 1: 
				// Уменьшение скорости, кол-ва фруктов на карте и её размеры
				if(configMap.speed > 1){ 
					configMap.speed--;
				} 
				break;
			case 2: 
				if(configMap.numFruits > 1){ 
					configMap.numFruits--;
				} 
				break;
			case 3:	
				if(configMap.mapSize > 0){ 
					configMap.mapSize--;
				} 
				break;
			case 4: 
				// Отключение препятствий
				configMap.border = false; 
				break;
			case 5: 
				// Отключение возможности к телепортации
				configMap.teleport = false; 
				break;
			};
			// Обновление меню настроек
			Display::Update(menuWidow);
			break;
		case KEY_RIGHT:
			switch(hiLight){
			case 1: 
				if(configMap.speed < 10){ 
					configMap.speed++;
				} 
				break;
			case 2: 
				if(configMap.numFruits < 99){ 
					configMap.numFruits++;
				} break;
			case 3: 
				if(configMap.mapSize < 2){ 
					configMap.mapSize++;
				} 
				break;
			case 4: 
				configMap.border = true; 
				break;
			case 5: 
				configMap.teleport = true; 
				break;
			};
			// Обновление меню настроек
			Display::Update(menuWidow);
			break;
		case KEY_EXIT: 
			FileSystem::SaveSettings(configMap,buttons); 
			return; 
			break;
		case KEY_ENTER: 
			if(hiLight == 0){ 
				// Сохранение настроек в файле при выходе из меню
				FileSystem::SaveSettings(configMap,buttons); 
				return; 
			}
			else if(hiLight == 6){
				// Настройки по умолчанию
				configMap = {8, 1, 1, false, false, false};
				// Обновление меню настроек
				Display::Update(menuWidow); 
			}
			else if(hiLight == 7){
				// Вывод меню об очистке рекордов
				if(PrintInfo(INFO_WIDTH - 6, INFO_HEIGHT,(char*)"CLEAR DATA ?", true)){
					configMap.clearScore = true;
				}
			}
			// Обновление экрана
			Display::Update();
			break;
		};
	}
}

// Меню настроек управления
void Menu::ControlSettingsLoop()
{
	// Элементы меню и названия клавиш курсора (можно было сделать через map контейнер)
	std::string menuPart[7] = {"Back", "Key Down:", "Key Up:", "Key Left:", "Key Right:", "Erase Settings"};
	std::string cursKey[4] = {"down ", "up  ", "left ", "right"};
	
	Display::Update(menuWidow);
	
	int hiLight = 0;
	while(1){
	
		Display::PrintScr(menuWidow, MENU_WIDTH/2 - 4, 0, (char*)"Controls", BLUE);
	
		for(int cursPosition = 0; cursPosition < 6; cursPosition++){
			// Подсветка курсора
			if(cursPosition == hiLight){ 
				wattron(menuWidow, COLOR_PAIR(GREEN));
			}
			// Отображение кнопок
			if(0 < cursPosition && cursPosition < 5){
				if(buttons[cursPosition - 1] < CURS_KEY_MIN || CURS_KEY_MAX < buttons[cursPosition - 1]){
					Display::PrintScr(menuWidow, MENU_WIDTH - 7, cursPosition + 2, buttons[cursPosition - 1]);
				}
				else{ 
					Display::PrintScr(menuWidow, MENU_WIDTH - 7, cursPosition + 2, (char*)cursKey[buttons[cursPosition - 1] - CURS_KEY_MIN].c_str());
				}	
			}
		
			if(cursPosition == 0){
				// Вывод параметра выхода из меню
				Display::PrintScr(menuWidow, 2, 1, (char*)menuPart[0].c_str());
			}
			else if(cursPosition == 5){ 
				// Вывод параметра очистки настроек
				Display::PrintScr(menuWidow, 2, MENU_HEIGHT - 3, (char*)menuPart[5].c_str());
			}
			else{ 
				Display::PrintScr(menuWidow, 2, cursPosition + 2, (char*)menuPart[cursPosition].c_str());
			}
		
			wattroff(menuWidow, COLOR_PAIR(GREEN));
		}
	
		switch (Periph::GetButton()){
		case KEY_UP: 
			if(hiLight > 0){
				hiLight--;
			} 
			break;
		case KEY_DOWN: 
			if(hiLight < 5){ 
				hiLight++;
			} 
			break;
		case KEY_EXIT:
			// Сохранение клавиш в файле при выходе из меню
			FileSystem::SaveSettings(configMap,buttons); 
			return; 
			break;
		case KEY_ENTER: 
			if(hiLight == 0){
				// Сохранение клавиш в файле при выходе из меню
				FileSystem::SaveSettings(configMap,buttons); 
				return; 
			}
			else if(hiLight == 5){
				// Сброс клавиш
				buttons[DOWN] = KEY_DOWN;
				buttons[UP] = KEY_UP;
				buttons[LEFT] = KEY_LEFT;
				buttons[RIGHT] = KEY_RIGHT;
			}
			else {
				// Создаем информационное окно для обработки нажатой клавиши
				PrintInfo(INFO_WIDTH, INFO_HEIGHT - 1, (char*)"Press the button!", false);
				cbreak();
				nodelay(stdscr, false);			// Отменяем задержку
				int recievedButton = getch();	// Принимаем символ
				nodelay(stdscr, true);			// Возобновляем задержку
				Display::DeleteWindow(infoWidow);	// Удаляем окно
				// Присваиваем символ
				switch(hiLight){
				case 1: 
					buttons[DOWN] = recievedButton; 
					break;
				case 2: 
					buttons[UP] = recievedButton; 
					break;
				case 3: 
					buttons[LEFT] = recievedButton; 
					break;
				case 4: 
					buttons[RIGHT] = recievedButton; 
					break;
				};
		
			}
			Display::Update(menuWidow); 
			break;
		};
	
	}
}

// Информация
void Menu::HelpLoop()
{
	// Названия клавиш курсора
	std::string cursKey[4] = {"down", "up", "left", "right"};
	
	int screenWidth = 0, screenHeight = 0;
	getmaxyx(stdscr, screenHeight, screenWidth);
	// Создаем информационное окно в центре экрана
	infoWidow = newwin(HELP_HEIGHT, HELP_WIDTH, (screenHeight - HELP_HEIGHT)/2, (screenWidth - HELP_WIDTH)/2);
	Display::Update(infoWidow);
	
	bool select = false;
	while(1){
		// Вывод информации об управлении и игре
		Display::PrintScr(infoWidow, HELP_WIDTH - 6, 0, (char*)"HELP", BLUE);
	
		if(!select){
			Display::PrintScr(infoWidow, 1, 0, (char*)"(1)", BLUE);
			Display::PrintScr(infoWidow, 2, 1, (char*)"Controls:", YELLOW);
			wattron(infoWidow, COLOR_PAIR(GREEN));
			Display::PrintScr(infoWidow, 2, 2, (char*)"Key Down:");
			Display::PrintScr(infoWidow, 2, 3, (char*)"Key Up:");
			Display::PrintScr(infoWidow, 2, 4, (char*)"Key Left:");
			Display::PrintScr(infoWidow, 2, 5, (char*)"Key Right:");
			Display::PrintScr(infoWidow, 2, 6, (char*)"Key Pause Game:");
			Display::PrintScr(infoWidow, 2, 7, (char*)"Key Help Game:");
			Display::PrintScr(infoWidow, 2, 8, (char*)"Key Quit Game:");
			wattroff(infoWidow, COLOR_PAIR(GREEN));
			Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, 6, 'p');
			Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, 7, 'h');
			Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, 8, 'q');
	
			// Вывод клавиш управления
			for(int currentButton = 0; currentButton < 4; currentButton++){
				if(buttons[currentButton] < CURS_KEY_MIN || CURS_KEY_MAX < buttons[currentButton]){
					Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, currentButton + 2, buttons[currentButton]);
				}
				else{ 
					Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, currentButton + 2, (char*)cursKey[buttons[currentButton]-CURS_KEY_MIN].c_str());
				}
			}
		}
		else {
			// Вывод справочной информации
			Display::PrintScr(infoWidow, 1, 0, (char*)"(2)", BLUE);
			Display::PrintScr(infoWidow, 2, 1, (char*)"About:", YELLOW);
			Display::PrintScr(infoWidow, 2, 2, (char*)"Game", GREEN);
			Display::PrintScr(infoWidow, 7, 2, 'T' | COLOR_PAIR(GREEN));
			Display::PrintScr(infoWidow, 8, 2, 'S' | COLOR_PAIR(GREEN));
			Display::PrintScr(infoWidow, 9, 2, 'N' | COLOR_PAIR(YELLOW));
			Display::PrintScr(infoWidow, 10, 2, 'A' | COLOR_PAIR(RED));
			Display::PrintScr(infoWidow, 11, 2, 'K' | COLOR_PAIR(GREEN));
			Display::PrintScr(infoWidow, 12, 2, 'E'| COLOR_PAIR(BLUE));
			Display::PrintScr(infoWidow, 14, 2, (char*)"v_1.3", BLUE);
			wattron(infoWidow, COLOR_PAIR(GREEN));
			Display::PrintScr(infoWidow, 2, 4, (char*)"Game created by DSuhoi (2020)");
			Display::PrintScr(infoWidow, 2, 5, (char*)"Email: <dsuh0i.h8@gmail.com>");
			Display::PrintScr(infoWidow, 2, 6, (char*)"Sourse Code:");
			Display::PrintScr(infoWidow, 2, 7, (char*)"https://github.com/DSuhoi/tsnake");
			Display::PrintScr(infoWidow, 5, 8, (char*)"Thanks for playing!");
			wattroff(infoWidow, COLOR_PAIR(GREEN));
		}
		switch(Periph::GetButton()){	//Выход из информационного меню
		case KEY_ENTER:
		case KEY_EXIT:
		case 'h':
			// Удаление информационного окна
			Display::DeleteWindow(infoWidow); 
			return; 
			break;
		case KEY_LEFT: 
			select = false; 
			Display::Update(infoWidow); 
			break;
		case KEY_RIGHT: 
			select = true; 
			Display::Update(infoWidow); 
			break;
		};
	}
}

// Окно паузы
int Menu::PauseLoop()
{
	// Элементы окна
	std::string pauseMenuStr[4] = {"Resume", "Restart", "Quit to Menu", "Quit Game"};
	
	int screenWidth = 0, screenHeight = 0;
	getmaxyx(stdscr, screenHeight, screenWidth);
	// Создание окна
	infoWidow = newwin(PAUSE_HEIGHT, PAUSE_WIDTH, (screenHeight - PAUSE_HEIGHT)/2, (screenWidth - PAUSE_WIDTH)/2);
	Display::Update(infoWidow);
	Display::PrintScr(infoWidow, PAUSE_WIDTH - 7, 0, (char*)"PAUSE", BLUE);
	// Подсветка меню
	int hiLight = 0;
	while(1){

		for(int i = 0; i < 4; i++){
			if(i == hiLight){
				wattron(infoWidow, COLOR_PAIR(GREEN));
			}
	
			Display::PrintScr(infoWidow, 2, i + 1, (char*)pauseMenuStr[i].c_str());
	
			wattroff(infoWidow, COLOR_PAIR(GREEN));
		}
	
		switch(Periph::GetButton()){
		case KEY_UP: if(hiLight > 0) hiLight--; break;
		case KEY_DOWN: if(hiLight < 3) hiLight++; break;
		case KEY_EXIT:
		case 'p': Display::DeleteWindow(infoWidow); return 0; break;
		case KEY_ENTER: 
			Display::DeleteWindow(infoWidow);
			switch(hiLight){	// Возвращение выбора
			case 0: 
				return 0; 
				break;
			case 1: 
				return 2; 
				break;
			case 2: 
				return 3; 
				break;
			case 3: 
				return 4; 
				break;
			}; 
			break;
		};
	}
}

// Вывод информации в отдельном окне
bool Menu::PrintInfo(int width, int height, char *stringWithInfo, bool select)
{
	int screenWidth, screenHeight;
	getmaxyx(stdscr, screenHeight, screenWidth);
	// Создаем информационное меню в центре экрана
	infoWidow = newwin(height, width, (screenHeight - height)/2, (screenWidth - width)/2);
	Display::Update(infoWidow);
	// и выводим текст
	Display::PrintScr(infoWidow, width - 6, 0, (char*)"INFO", BLUE);
	Display::PrintScr(infoWidow, 1, 1, stringWithInfo, BLUE);
	
	if(select){	// Если это вопрос (с выбором ответа в виде "Да" или "Нет")
		wattron(infoWidow, COLOR_PAIR(GREEN));
		std::string selectStr[2] = {"<No> ", "<Yes>"};
		bool hiLight = false;
		while(1){
			Display::PrintScr(infoWidow, (width/2 - 2), height - 2,(char*)selectStr[hiLight].c_str());
			switch (Periph::GetButton()){	// Обработка нажатых клавиш
			case KEY_LEFT: 
				hiLight = false; 
				break;
			case KEY_RIGHT: 
				hiLight = true; 
				break;
			case KEY_ENTER: 
				wattroff(infoWidow, COLOR_PAIR(GREEN)); 
				Display::DeleteWindow(infoWidow); 
				return hiLight; // Возвращаем ответ
				break;
			};
		} 
	}
	return false;
}

// Возвращаем настройки карты
CONFIG& Menu::GetConfigMap()
{ 
	return configMap; 
}	

// Возвращаем настройки управления
int *Menu::GetControl()
{ 
	return buttons; 
}
