#include <iostream>
#include <fstream>
#include "../term/display.h"
#include "menu.h"


//инициализация главного меню
void Menu::InitMainMenu()
{
	int x, y;	//размеры экран X*Y
	getmaxyx(stdscr, y, x);
	//создаем окно
	menuWidow = newwin(MENU_HEIGHT, MENU_WIDTH,(y - MENU_HEIGHT)/2, (x - MENU_WIDTH)/2);
	//настройки 
	buttons[DOWN] = KEY_DOWN;
	buttons[UP] = KEY_UP;
	buttons[LEFT] = KEY_LEFT;
	buttons[RIGHT] = KEY_RIGHT;
	
	configMap = LoadSettings(buttons);
}

//цикл главного меню
int Menu::MainMenuLoop()
{
	int hiLight = 0;	//выбор элемента меню

	//массив из названий элементов меню
	std::string menuPart[6] ={"Start Game", "Load Map", "Lvl Settings", "Controls", "Help", "Exit"};
	
	Display::Update(menuWidow);	//очищаем и обновляем окно
	//название окна
	
	while(1){
	
	Display::PrintScr(menuWidow, MENU_WIDTH/2 - 5, 0, (char*)"TSNAKE 1.0", BLUE);
	
		//зелёные элементы
	
	for(int i=0; i<6;i++){
		if(i==hiLight)	//выбранную строку подсвечиваем
			wattron(menuWidow, COLOR_PAIR(GREEN));
		//выводим элементы массива	
		Display::PrintScr(menuWidow, 2, i+1, (char*)menuPart[i].c_str());
		
			wattroff(menuWidow, COLOR_PAIR(GREEN));
	}
	

	
	switch(periph()){	//читаем клавиши
	case KEY_UP: if(hiLight>0) hiLight--; break;	//выбор элемента меню
	case KEY_DOWN: if(hiLight<5) hiLight++; break;
	case KEY_EXIT: return 0; break;		//выход из программы
	case KEY_ENTER:
		switch (hiLight){
		case 0: return 1; break;	//если это игра, то выходим из меню
		case 1: if(SearchMap()) return 2; break;
		case 2: LvlSettingsLoop();  break;		//настройки уровня
		case 3: ControlSettingsLoop(); break;	//настройки управления
		case 4: HelpLoop(); break;				//информация
		case 5: return 0; break;				//выход из программы
		}; Display::Update(menuWidow); break;
	};
	}
}
bool Menu::SearchMap()
{
	int x, y;
	
	for(int i=0; i<STR_LEN; i++) nameFile[i] = 0;	//стираем символьный массив
	
	getmaxyx(stdscr, y, x);
	//создаем информационное окно в центре экрана
	infoWidow = newwin(INFO_HEIGHT-1, INFO_WIDTH+15, (y-INFO_HEIGHT+1)/2, (x-INFO_WIDTH-15)/2);
	Display::Update(infoWidow);
	Display::PrintScr(infoWidow, (INFO_WIDTH+15)/2 - 4, 0, (char*)"Load Map", BLUE);
	
	Display::PrintScr(infoWidow, 2, 1, (char*)"File name: ", GREEN);
	
	echo();	//режим отображения ввода включён
	wscanw(infoWidow,"%s", nameFile);	//считываем файл
	noecho();
	Display::DeleteWindow(infoWidow);
	if(nameFile[0]==0) return false;	//если ничего не ввели
	std::ifstream fin(FOLDER_FILE + nameFile);
	if(!fin){	//если файл закрыт
		PrintInfo(false, INFO_WIDTH+5, INFO_HEIGHT-1, (char*)"ERROR: File not found!");
		getchar();
		Display::DeleteWindow(infoWidow);
		return false;
	}
	fin.close();
	return true;
	
}

char* Menu::GetNameFile()
{ 
	return nameFile; 
}

//меню настроек игры
void Menu::LvlSettingsLoop()
{

	int hiLight = 0;
	
	bool selectCh = 0;	//вид скобок
	
	//элементы меню, параметры карты и ответы
	std::string menuPart[8] = {"Back", "Speed:", "Fruit:","Map Size:", 
		"Border:", "Teleport:", "Erase Settings", "Erase Records"};
	std::string mapSize[3] = {"Small", "Medium", "Large"};
	std::string selectStr[2] = {"Off", "On"};
	
	//левые и правые скобки
	char ChL[] = {'[', '<'};
	char ChR[] = {']', '>'};
	
	char buffStr[9];	//символьный массив под ответ
	
	Display::Update(menuWidow);
	
	while(1){
	
	Display::PrintScr(menuWidow, MENU_WIDTH/2 - 6, 0, (char*)"Lvl Settings", BLUE);
		
	for(int i=0; i<8;i++){
		if(i==hiLight){
			wattron(menuWidow, COLOR_PAIR(GREEN));
			selectCh = 1;
		} else selectCh = 0;
		
		switch(i){
		case 1: case 2:	//если это численный параметр
			sprintf(buffStr,"%c%d%c", ChL[selectCh],(i==1) ? configMap.speed : configMap.fruitSize , ChR[selectCh]);
			Display::PrintScr(menuWidow, MENU_WIDTH-7, i+2, buffStr); break;
		case 3:			//если это строка
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)mapSize[configMap.mapSize].c_str(), ChR[selectCh]);
			Display::PrintScr(menuWidow, MENU_WIDTH-9, i+2, buffStr); break;
		case 4:
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)selectStr[configMap.border].c_str(), ChR[selectCh]);
			Display::PrintScr(menuWidow, MENU_WIDTH-7, i+2, buffStr); break;
		case 5:
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)selectStr[configMap.teleport].c_str(), ChR[selectCh]);
			Display::PrintScr(menuWidow, MENU_WIDTH-7, i+2, buffStr); break;
		};
		
		
		
		//элемент выхода из меню
		if(i==0)
			Display::PrintScr(menuWidow, 2, 1, (char*)menuPart[0].c_str());
		//элемент очистки настроек
		else if(i>5) Display::PrintScr(menuWidow, 2, MENU_HEIGHT-9+i, (char*)menuPart[i].c_str());
		
		else Display::PrintScr(menuWidow, 2, i+2, (char*)menuPart[i].c_str());
		
		
		//очистка массива
		for(int i=0; i<9; i++) buffStr[i] = 0;
		
		wattroff(menuWidow, COLOR_PAIR(GREEN));
	}
	
	switch(periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<7) hiLight++; break;
	case KEY_LEFT: //в зависимости от элемента настраиваем параметры
			switch(hiLight){
			case 1: if(configMap.speed>1) configMap.speed--; break;
			case 2: if(configMap.fruitSize>1) configMap.fruitSize--; break;
			case 3:	if(configMap.mapSize>0) configMap.mapSize--; break;
			case 4: configMap.border = false; break;
			case 5: configMap.teleport = false; break;
			}; Display::Update(menuWidow);
				break;
	case KEY_RIGHT:
			switch(hiLight){
			case 1: if(configMap.speed<10) configMap.speed++; break;
			case 2: if(configMap.fruitSize<99) configMap.fruitSize++; break;
			case 3: if(configMap.mapSize<2) configMap.mapSize++; break;
			case 4: configMap.border = true; break;
			case 5: configMap.teleport = true; break;
			}; Display::Update(menuWidow);
				break;
	case KEY_EXIT: SaveSettings(configMap,buttons); return; break;
	case KEY_ENTER: 
			if(hiLight==0){ 
				SaveSettings(configMap,buttons); 
				return; 
			}
			else if(hiLight==6){ 
				configMap = {8, 1, 1, false, false, false}; 
				Display::Update(menuWidow); 
			}
			else if(hiLight==7){
				if(PrintInfo(true, INFO_WIDTH-6, INFO_HEIGHT,(char*)"CLEAR DATA ?"))
					configMap.clearScore = true;
			}
			Display::Update();
			break;
	};
	}
}

//меню настроек управления
void Menu::ControlSettingsLoop()
{
	int hiLight = 0;
	
	//элементы меню и названия клавиш курсора (можно было сделать через map контейнер)
	std::string menuPart[7] = {"Back", "Key Down:", "Key Up:", "Key Left:", "Key Right:", "Erase Settings"};
	std::string cursKey[4] = {"down ", "up  ", "left ", "right"};
	
	Display::Update(menuWidow);
	
	while(1){
	
	Display::PrintScr(menuWidow, MENU_WIDTH/2 - 4, 0, (char*)"Controls", BLUE);
	
	for(int i=0; i<6;i++){
		
		if(i==hiLight) wattron(menuWidow, COLOR_PAIR(GREEN));
		
		
		if(i<5 && i>0){
		if(buttons[i-1]<CURS_KEY_MIN || buttons[i-1]>CURS_KEY_MAX)
			Display::PrintScr(menuWidow, MENU_WIDTH-7, i+2, buttons[i-1]);
		else Display::PrintScr(menuWidow, MENU_WIDTH-7, i+2, (char*)cursKey[buttons[i-1]-CURS_KEY_MIN].c_str());
			
		}
		
		
		
		//элемент выхода из меню
		if(i==0) Display::PrintScr(menuWidow, 2, 1, (char*)menuPart[0].c_str());
		//элемент очистки настроек
		else if(i==5) Display::PrintScr(menuWidow, 2, MENU_HEIGHT-3, (char*)menuPart[5].c_str());
		
		else Display::PrintScr(menuWidow, 2, i+2, (char*)menuPart[i].c_str());
		
		
		wattroff(menuWidow, COLOR_PAIR(GREEN));
	}
	
	switch (periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<5) hiLight++; break;
	case KEY_EXIT: SaveSettings(configMap,buttons); return; break;
	case KEY_ENTER: 
		if(hiLight==0){ SaveSettings(configMap,buttons); return; }
		else if(hiLight==5){
			buttons[DOWN] = KEY_DOWN;
			buttons[UP] = KEY_UP;
			buttons[LEFT] = KEY_LEFT;
			buttons[RIGHT] = KEY_RIGHT;
		}
		else {	//создаем информационное окно для обработки нажатой клавиши
		PrintInfo(false, INFO_WIDTH, INFO_HEIGHT-1, (char*)"Press the button!");
		cbreak();
		nodelay(stdscr, false);	//отменяем задержку
		int ch = getch();	//принимаем символ
		nodelay(stdscr, true);
		Display::DeleteWindow(infoWidow);	//удаляем окно
		
		switch(hiLight){	//присваиваем символ
		case 1: buttons[DOWN] = ch; break;
		case 2: buttons[UP] = ch; break;
		case 3: buttons[LEFT] = ch; break;
		case 4: buttons[RIGHT] = ch; break;
		};
		
		} Display::Update(menuWidow); break;
	};
	
	}
}

//информация
void Menu::HelpLoop()
{
	int x, y;
	bool select = false;
	getmaxyx(stdscr, y, x);
	//создаем информационное окно в центре экрана
	infoWidow = newwin(HELP_HEIGHT, HELP_WIDTH, (y-HELP_HEIGHT)/2, (x-HELP_WIDTH)/2);
	
	//названия клавиш курсора
	std::string cursKey[4] = {"down", "up", "left", "right"};
	
	Display::Update(infoWidow);
	
	while(1){
	//вывод информации об управлении и игре
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
	
	//обработка клавиш управления
	
	for(int i = 0; i<4; i++)
		if(buttons[i]<CURS_KEY_MIN || buttons[i]>CURS_KEY_MAX)
			Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, i+2, buttons[i]);
		else Display::PrintScr(infoWidow, Help_WIDTH_RIGHT, i+2, (char*)cursKey[buttons[i]-CURS_KEY_MIN].c_str());
	}
	
	else {
		Display::PrintScr(infoWidow, 1, 0, (char*)"(2)", BLUE);
		Display::PrintScr(infoWidow, 2, 1, (char*)"About:", YELLOW);
		Display::PrintScr(infoWidow, 2, 2, (char*)"Game", GREEN);
		Display::PrintScr(infoWidow, 7, 2,'T' | COLOR_PAIR(GREEN));
		Display::PrintScr(infoWidow, 8, 2,'S' | COLOR_PAIR(GREEN));
		Display::PrintScr(infoWidow, 9, 2,'N' | COLOR_PAIR(YELLOW));
		Display::PrintScr(infoWidow, 10, 2,'A' | COLOR_PAIR(RED));
		Display::PrintScr(infoWidow, 11, 2,'K' | COLOR_PAIR(GREEN));
		Display::PrintScr(infoWidow, 12, 2,'E'| COLOR_PAIR(BLUE));
		Display::PrintScr(infoWidow, 14, 2, (char*)"v_1.3", BLUE);
		wattron(infoWidow, COLOR_PAIR(GREEN));
		Display::PrintScr(infoWidow, 2, 4, (char*)"Game created by DSuhoi (2020)");
		Display::PrintScr(infoWidow, 2, 5, (char*)"Email: <dsuh0i.h8@gmail.com>");
		Display::PrintScr(infoWidow, 2, 6, (char*)"Sourse Code:");
		Display::PrintScr(infoWidow, 2, 7, (char*)"https://github.com/DSuhoi/tsnake");
		Display::PrintScr(infoWidow, 5, 8, (char*)"Thanks for playing!");
		wattroff(infoWidow, COLOR_PAIR(GREEN));
	}
	switch(periph()){	//выход из информационного меню
	case KEY_ENTER:
	case KEY_EXIT:
	case 'h': Display::DeleteWindow(infoWidow); return; break;
	case KEY_LEFT: select = false; Display::Update(infoWidow); break;
	case KEY_RIGHT: select = true; Display::Update(infoWidow); break;
	};
	}
	
}

//окно паузы
int Menu::PauseLoop()
{
	int hiLight = 0;
	int x, y;
	getmaxyx(stdscr, y, x);
	//создание окна
	infoWidow = newwin(PAUSE_HEIGHT, PAUSE_WIDTH, (y-PAUSE_HEIGHT)/2, (x-PAUSE_WIDTH)/2);
	//элементы окна
	std::string pauseMenuStr[4] = {"Resume", "Restart", "Quit to Menu", "Quit Game"};

	Display::Update(infoWidow);
		
	Display::PrintScr(infoWidow, PAUSE_WIDTH - 7, 0, (char*)"PAUSE", BLUE);
	
	while(1){

	for(int i=0; i<4; i++){
	if(i==hiLight) wattron(infoWidow, COLOR_PAIR(GREEN));
	
	Display::PrintScr(infoWidow, 2, i+1, (char*)pauseMenuStr[i].c_str());
	
	wattroff(infoWidow, COLOR_PAIR(GREEN));
	}
	
	switch(periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<3) hiLight++; break;
	case KEY_EXIT:
	case 'p': Display::DeleteWindow(infoWidow); return 0; break;
	case KEY_ENTER: 
				Display::DeleteWindow(infoWidow);
				switch(hiLight){	//возвращение выбора
				case 0: return 0; break;
				case 1: return 2; break;
				case 2: return 3; break;
				case 3: return 4; break;
				}; break;
	};
	
	}
}

//вывод информации в отдельном окне
bool Menu::PrintInfo(bool isSelect, int w, int h, char *buff)
{
	int _x, _y;
	getmaxyx(stdscr, _y, _x);
	//создаем информационное меню в центре экрана
	infoWidow = newwin(h, w, (_y-h)/2, (_x-w)/2);
	
	Display::Update(infoWidow);
	//и выводим текст
	Display::PrintScr(infoWidow, w - 6, 0, (char*)"INFO", BLUE);
	Display::PrintScr(infoWidow, 1, 1, buff, BLUE);
	
	if(isSelect){	//если это вопрос
		
		wattron(infoWidow, COLOR_PAIR(GREEN));
		std::string selectStr[2] = {"<No> ", "<Yes>"};
		bool hiLight = false;

	while(1){
		
	Display::PrintScr(infoWidow, (w/2 - 2), h-2,(char*)selectStr[hiLight].c_str());
		
	switch (periph()){	//возвращаем ответ
	case KEY_LEFT: hiLight = false; break;
	case KEY_RIGHT: hiLight = true; break;
	case KEY_ENTER: wattroff(infoWidow, COLOR_PAIR(GREEN)); Display::DeleteWindow(infoWidow); return hiLight; break;
	};
	} }
	
	return false;
}

//возвращаем настройки карты
CONFIG& Menu::GetConfigMap()
{ 
	return configMap; 
}	

//возвращаем настройки управления
int* Menu::SetControl()
{ 
	return buttons; 
} 
