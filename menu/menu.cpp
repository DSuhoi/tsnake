#include <iostream>
#include <fstream>
#include "../term/display.h"
#include "menu.h"


//инициализация главного меню
void Menu::initMainMenu(){
	int x, y;	//размеры экран X*Y
	getmaxyx(stdscr, y, x);
	//создаем окно
	menu = newwin(MenuHeight, MenuWidth,(y - MenuHeight)/2, (x - MenuWidth)/2);
	//настройки 
	buttons[DOWN] = KEY_DOWN;
	buttons[UP] = KEY_UP;
	buttons[LEFT] = KEY_LEFT;
	buttons[RIGHT] = KEY_RIGHT;
	
	conf = LoadSettings(buttons);
}

//цикл главного меню
int Menu::MainMenuLoop(){
	
	int hiLight = 0;	//выбор элемента меню

	//массив из названий элементов меню
	std::string menuPart[6] ={"Start Game", "Load Map", "Lvl Settings", "Controls", "Help", "Exit"};
	
	update(menu);	//очищаем и обновляем окно
	//название окна
	
	while(1){
	
	printScr(menu, MenuWidth/2 - 5, 0, (char*)"TSNAKE 1.0", BLUE);
	
		//зелёные элементы
	
	for(int i=0; i<6;i++){
		if(i==hiLight)	//выбранную строку подсвечиваем
			wattron(menu, COLOR_PAIR(GREEN));
		//выводим элементы массива	
		printScr(menu, 2, i+1, (char*)menuPart[i].c_str());
		
			wattroff(menu, COLOR_PAIR(GREEN));
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
		}; update(menu); break;
	};
	}
}
bool Menu::SearchMap(){
	int x, y;
	
	for(int i=0; i<STRLEN; i++) NameFile[i] = 0;	//стираем символьный массив
	
	getmaxyx(stdscr, y, x);
	//создаем информационное окно в центре экрана
	info = newwin(InfoHeight-1, InfoWidth+15, (y-InfoHeight+1)/2, (x-InfoWidth-15)/2);
	update(info);
	printScr(info, (InfoWidth+15)/2 - 4, 0, (char*)"Load Map", BLUE);
	
	printScr(info, 2, 1, (char*)"File name: ", GREEN);
	
	echo();	//режим отображения ввода включён
	wscanw(info,"%s", NameFile);	//считываем файл
	noecho();
	deleteWindow(info);
	if(NameFile[0]==0) return false;	//если ничего не ввели
	std::ifstream fin(FOLDER_FILE + NameFile);
	if(!fin){	//если файл закрыт
		PrintInfo(false, InfoWidth+5, InfoHeight-1, (char*)"ERROR: File not found!");
		getchar();
		deleteWindow(info);
		return false;
	}
	fin.close();
	return true;
	
}

void Menu::SelectCustomMap(Map &map){
	int Size;
	LoadMap(NameFile, Size, map);	//если такого файла нет
	conf.mapSize = Size;	//присваиваем переменную
}


//меню настроек игры
void Menu::LvlSettingsLoop(){

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
	
	update(menu);
	
	while(1){
	
	printScr(menu, MenuWidth/2 - 6, 0, (char*)"Lvl Settings", BLUE);
		
	for(int i=0; i<8;i++){
		if(i==hiLight){
			wattron(menu, COLOR_PAIR(GREEN));
			selectCh = 1;
		} else selectCh = 0;
		
		switch(i){
		case 1: case 2:	//если это численный параметр
			sprintf(buffStr,"%c%d%c", ChL[selectCh],(i==1) ? conf.speed : conf.fruitSize , ChR[selectCh]);
			printScr(menu, MenuWidth-7, i+2, buffStr); break;
		case 3:			//если это строка
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)mapSize[conf.mapSize].c_str(), ChR[selectCh]);
			printScr(menu, MenuWidth-9, i+2, buffStr); break;
		case 4:
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)selectStr[conf.border].c_str(), ChR[selectCh]);
			printScr(menu, MenuWidth-7, i+2, buffStr); break;
		case 5:
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)selectStr[conf.teleport].c_str(), ChR[selectCh]);
			printScr(menu, MenuWidth-7, i+2, buffStr); break;
		};
		
		
		
		//элемент выхода из меню
		if(i==0) printScr(menu, 2, 1, (char*)menuPart[0].c_str());
		//элемент очистки настроек
		else if(i>5) printScr(menu, 2, MenuHeight-9+i, (char*)menuPart[i].c_str());
		
		else printScr(menu, 2, i+2, (char*)menuPart[i].c_str());
		
		
		//очистка массива
		for(int i=0; i<9; i++) buffStr[i] = 0;
		
		wattroff(menu, COLOR_PAIR(GREEN));
	}
	
	switch(periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<7) hiLight++; break;
	case KEY_LEFT: //в зависимости от элемента настраиваем параметры
			switch(hiLight){
			case 1: if(conf.speed>1) conf.speed--; break;
			case 2: if(conf.fruitSize>1) conf.fruitSize--; break;
			case 3:	if(conf.mapSize>0) conf.mapSize--; break;
			case 4: conf.border = false; break;
			case 5: conf.teleport = false; break;
			}; update(menu);
				break;
	case KEY_RIGHT:
			switch(hiLight){
			case 1: if(conf.speed<10) conf.speed++; break;
			case 2: if(conf.fruitSize<99) conf.fruitSize++; break;
			case 3: if(conf.mapSize<2) conf.mapSize++; break;
			case 4: conf.border = true; break;
			case 5: conf.teleport = true; break;
			}; update(menu);
				break;
	case KEY_EXIT: SaveSettings(conf,buttons); return; break;
	case KEY_ENTER: 
			if(hiLight==0){ SaveSettings(conf,buttons); return; }
			else if(hiLight==6){ conf = {8, 1, 1, false, false, false}; update(menu); }
			else if(hiLight==7){
				if(PrintInfo(true, InfoWidth-6, InfoHeight,(char*)"CLEAR DATA ?"))
					conf.clearScore = true;
			}
			update();
			break;
	};
	}
}

//меню настроек управления
void Menu::ControlSettingsLoop(){
	
	int hiLight = 0;
	
	//элементы меню и названия клавиш курсора (можно было сделать через map контейнер)
	std::string menuPart[7] = {"Back", "Key Down:", "Key Up:", "Key Left:", "Key Right:", "Erase Settings"};
	std::string cursKey[4] = {"down ", "up  ", "left ", "right"};
	
	update(menu);
	
	while(1){
	
	printScr(menu, MenuWidth/2 - 4, 0, (char*)"Controls", BLUE);
	
	for(int i=0; i<6;i++){
		
		if(i==hiLight) wattron(menu, COLOR_PAIR(GREEN));
		
		
		if(i<5 && i>0){
		if(buttons[i-1]<CURS_KEY_MIN || buttons[i-1]>CURS_KEY_MAX)
			printScr(menu, MenuWidth-7, i+2, buttons[i-1]);
		else printScr(menu, MenuWidth-7, i+2, (char*)cursKey[buttons[i-1]-CURS_KEY_MIN].c_str());
			
		}
		
		
		
		//элемент выхода из меню
		if(i==0) printScr(menu, 2, 1, (char*)menuPart[0].c_str());
		//элемент очистки настроек
		else if(i==5) printScr(menu, 2, MenuHeight-3, (char*)menuPart[5].c_str());
		
		else printScr(menu, 2, i+2, (char*)menuPart[i].c_str());
		
		
		wattroff(menu, COLOR_PAIR(GREEN));
	}
	
	switch (periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<5) hiLight++; break;
	case KEY_EXIT: SaveSettings(conf,buttons); return; break;
	case KEY_ENTER: 
		if(hiLight==0){ SaveSettings(conf,buttons); return; }
		else if(hiLight==5){
			buttons[DOWN] = KEY_DOWN;
			buttons[UP] = KEY_UP;
			buttons[LEFT] = KEY_LEFT;
			buttons[RIGHT] = KEY_RIGHT;
		}
		else {	//создаем информационное окно для обработки нажатой клавиши
		PrintInfo(false, InfoWidth, InfoHeight-1, (char*)"Press the button!");
		cbreak();
		nodelay(stdscr, false);	//отменяем задержку
		int ch = getch();	//принимаем символ
		nodelay(stdscr, true);
		deleteWindow(info);	//удаляем окно
		
		switch(hiLight){	//присваиваем символ
		case 1: buttons[DOWN] = ch; break;
		case 2: buttons[UP] = ch; break;
		case 3: buttons[LEFT] = ch; break;
		case 4: buttons[RIGHT] = ch; break;
		};
		
		} update(menu); break;
	};
	
	}
}

//информация
void Menu::HelpLoop(){
	
	int x, y;
	bool select = false;
	getmaxyx(stdscr, y, x);
	//создаем информационное окно в центре экрана
	info = newwin(HelpHeight, HelpWidth, (y-HelpHeight)/2, (x-HelpWidth)/2);
	
	//названия клавиш курсора
	std::string cursKey[4] = {"down", "up", "left", "right"};
	
	update(info);
	
	while(1){
	//вывод информации об управлении и игре
	printScr(info, HelpWidth - 6, 0, (char*)"HELP", BLUE);
	
	if(!select){
	printScr(info, 1, 0, (char*)"(1)", BLUE);
	printScr(info, 2, 1, (char*)"Controls:", YELLOW);
	wattron(info, COLOR_PAIR(GREEN));
	printScr(info, 2, 2, (char*)"Key Down:");
	printScr(info, 2, 3, (char*)"Key Up:");
	printScr(info, 2, 4, (char*)"Key Left:");
	printScr(info, 2, 5, (char*)"Key Right:");
	printScr(info, 2, 6, (char*)"Key Pause Game:");
	printScr(info, 2, 7, (char*)"Key Help Game:");
	printScr(info, 2, 8, (char*)"Key Quit Game:");
	wattroff(info, COLOR_PAIR(GREEN));
	printScr(info, HelpWidthRight, 6, 'p');
	printScr(info, HelpWidthRight, 7, 'h');
	printScr(info, HelpWidthRight, 8, 'q');
	
	//обработка клавиш управления
	
	for(int i = 0; i<4; i++)
		if(buttons[i]<CURS_KEY_MIN || buttons[i]>CURS_KEY_MAX)
			printScr(info, HelpWidthRight, i+2, buttons[i]);
		else printScr(info, HelpWidthRight, i+2, (char*)cursKey[buttons[i]-CURS_KEY_MIN].c_str());
	}
	
	else {
		printScr(info, 1, 0, (char*)"(2)", BLUE);
		printScr(info, 2, 1, (char*)"About:", YELLOW);
		printScr(info, 2, 2, (char*)"Game", GREEN);
		printScr(info, 7, 2,'T' | COLOR_PAIR(GREEN));
		printScr(info, 8, 2,'S' | COLOR_PAIR(GREEN));
		printScr(info, 9, 2,'N' | COLOR_PAIR(YELLOW));
		printScr(info, 10, 2,'A' | COLOR_PAIR(RED));
		printScr(info, 11, 2,'K' | COLOR_PAIR(GREEN));
		printScr(info, 12, 2,'E'| COLOR_PAIR(BLUE));
		printScr(info, 14, 2, (char*)"v_1.0", BLUE);
		wattron(info, COLOR_PAIR(GREEN));
		printScr(info, 2, 4, (char*)"Game created by DSuhoi (2020)");
		printScr(info, 2, 5, (char*)"Email: <dsuh0i.h8@gmail.com>");
		printScr(info, 2, 6, (char*)"Sourse Code:");
		printScr(info, 2, 7, (char*)"https://github.com/DSuhoi/tsnake");
		printScr(info, 5, 8, (char*)"Thanks for playing!");
		wattroff(info, COLOR_PAIR(GREEN));
	}
	switch(periph()){	//выход из информационного меню
	case KEY_ENTER:
	case KEY_EXIT:
	case 'h': deleteWindow(info); return; break;
	case KEY_LEFT: select = false; update(info); break;
	case KEY_RIGHT: select = true; update(info); break;
	};
	}
	
}

//окно паузы
int Menu::PauseLoop(){
	
	int hiLight = 0;
	int x, y;
	getmaxyx(stdscr, y, x);
	//создание окна
	info = newwin(PauseHeight, PauseWidth, (y-PauseHeight)/2, (x-PauseWidth)/2);
	//элементы окна
	std::string pauseMenuStr[4] = {"Resume", "Restart", "Quit to Menu", "Quit Game"};

	update(info);
	
	printScr(info, PauseWidth - 7, 0, (char*)"PAUSE", BLUE);
	
	while(1){
	
	for(int i=0; i<4; i++){
	if(i==hiLight) wattron(info, COLOR_PAIR(GREEN));
	
	printScr(info, 2, i+1, (char*)pauseMenuStr[i].c_str());
	
	wattroff(info, COLOR_PAIR(GREEN));
	}
	
	switch(periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<3) hiLight++; break;
	case KEY_EXIT:
	case 'p': deleteWindow(info); return 0; break;
	case KEY_ENTER: 
				deleteWindow(info);
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
bool Menu::PrintInfo(bool isSelect, int w, int h, char *buff){
	
	int _x, _y;
	getmaxyx(stdscr, _y, _x);
	//создаем информационное меню в центре экрана
	info = newwin(h, w, (_y-h)/2, (_x-w)/2);
	
	update(info);
	//и выводим текст
	printScr(info, w - 6, 0, (char*)"INFO", BLUE);
	printScr(info, 1, 1, buff, BLUE);
	
	if(isSelect){	//если это вопрос
		
		wattron(info, COLOR_PAIR(GREEN));
		std::string selectStr[2] = {"<No> ", "<Yes>"};
		bool hiLight = false;

	while(1){
		
	printScr(info, (w/2 - 2), h-2,(char*)selectStr[hiLight].c_str());
		
	switch (periph()){	//возвращаем ответ
	case KEY_LEFT: hiLight = false; break;
	case KEY_RIGHT: hiLight = true; break;
	case KEY_ENTER: wattroff(info, COLOR_PAIR(GREEN)); deleteWindow(info); return hiLight; break;
	};
	} }
	
	return false;
}


CONFIG& Menu::getConfig(){ return conf; }	//возвращаем настройки карты

int* Menu::setControl(){ return buttons; } //возвращаем настройки управления
