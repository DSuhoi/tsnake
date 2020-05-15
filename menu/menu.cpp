#include <iostream>
#include <ncurses.h>
#include <string>
#include "../display/display.h"
#include "../periph/periph.h"
#include "menu.h"

//инициализация главного меню
void Menu::initMainMenu(){
	int x, y;	//размеры экран X*Y
	getmaxyx(stdscr, y, x);
	//создаем окно
	menu = newwin(MenuHeight, MenuWidth,(y - MenuHeight)/2, (x - MenuWidth)/2);
	//info = NULL;	//указатель временно не используется
	//настройки 
	buttons = {KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN};
	conf = {8, 1, 1, 0, 0};
}

//цикл главного меню
bool Menu::MainMenuLoop(){

	int hiLight = 0;	//выбор элемента меню

	//массив из названий элементов меню
	std::string menuPart[5] ={"Start Game", "Lvl Settings", "Controls", "Help", "Exit"};
	
	while(1){
		
	cls(menu);		//очищаем и обновляем окно
	update(menu);
	//название окна
	Display::printScr(menu, MenuWidth/2 - 5, 0, (char*)"TSNAKE 1.0", BLUE);
	
	wattron(menu, COLOR_PAIR(GREEN));	//зелёные элементы
	
	for(int i=0; i<5;i++){
		if(i==hiLight)	//выбранную строку подсвечиваем
			wattron(menu, A_BOLD);
		//выводим элементы массива	
		Display::printScr(menu, 2, i+1, (char*)menuPart[i].c_str());
		
		wattroff(menu, A_BOLD);
	}
	
	wattroff(menu, COLOR_PAIR(GREEN));
	
	switch(periph()){	//читаем клавиши
	case KEY_UP: if(hiLight>0) hiLight--; break;	//выбор элемента меню
	case KEY_DOWN: if(hiLight<4) hiLight++; break;
	case KEY_EXIT: return true; break;		//выход из программы
	case KEY_ENTER:
		switch (hiLight){
		case 0: return false; break;	//если это игра, то выходим из меню
		case 1: LvlSettingsLoop(); break;	//настройки уровня
		case 2: ControlSettingsLoop(); break;	//настройки управления
		case 3: HelpLoop(); break;				//информация
		case 4: return true; break;				//выход из программы
		}; break;
	};
	}
}

//меню настроек игры
void Menu::LvlSettingsLoop(){

	int hiLight = 0;
	
	bool selectCh = 0;	//вид скобок
	
	//элементы меню, параметры карты и ответы
	std::string menuPart[7] = {"Back", "Speed:", "Fructs:","Map Size:", "Border:", "Teleport:", "Erase Settings"};
	std::string mapSize[3] = {"Small", "Medium", "Large"};
	std::string selectStr[2] = {"No", "Yes"};
	
	//левые и правые скобки
	char ChL[] = {'[', '<'};
	char ChR[] = {']', '>'};
	
	char buffStr[9];	//символьный массив под ответ
	
	while(1){
		
	cls(menu);
	update(menu);
	
	Display::printScr(menu, MenuWidth/2 - 6, 0, (char*)"Lvl Settings", BLUE);
		
	for(int i=0; i<7;i++){
		if(i==hiLight){
			wattron(menu, A_BOLD);
			selectCh = 1;
		} else selectCh = 0;
		
		switch(i){
		case 1: case 2:	//если это численный параметр
			sprintf(buffStr,"%c%d%c", ChL[selectCh],(i==1) ? conf.speed : conf.fructSize , ChR[selectCh]);
			Display::printScr(menu, MenuWidth-7, i+2, buffStr); break;
		case 3:			//если это 
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)mapSize[conf.mapSize].c_str(), ChR[selectCh]);
			Display::printScr(menu, MenuWidth-9, i+2, buffStr); break;
		case 4:
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)selectStr[conf.border].c_str(), ChR[selectCh]);
			Display::printScr(menu, MenuWidth-7, i+2, buffStr); break;
		case 5:
			sprintf(buffStr,"%c%s%c", ChL[selectCh], (char*)selectStr[conf.teleport].c_str(), ChR[selectCh]);
			Display::printScr(menu, MenuWidth-7, i+2, buffStr); break;
		};
		
		wattron(menu, COLOR_PAIR(GREEN));
		
		//элемент выхода из меню
		if(i==0) Display::printScr(menu, 2, 1, (char*)menuPart[0].c_str());
		//элемент очистки настроек
		else if(i==6) Display::printScr(menu, 2, MenuHeight-3, (char*)menuPart[6].c_str());
		
		else Display::printScr(menu, 2, i+2, (char*)menuPart[i].c_str());
		
		
		//очистка массива
		for(int i=0; i<9; i++) buffStr[i] = 0;
		
		wattroff(menu, A_BOLD | COLOR_PAIR(GREEN));
	}
	
	switch(periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<6) hiLight++; break;
	case KEY_LEFT: //в зависимости от элемента настраиваем параметры
			switch(hiLight){
			case 1: if(conf.speed>1) conf.speed--; break;
			case 2: if(conf.fructSize>1) conf.fructSize--; break;
			case 3:	if(conf.mapSize>0) conf.mapSize--; break;
			case 4: conf.border = false; break;
			case 5: conf.teleport = false; break;
			};
				break;
	case KEY_RIGHT:
			switch(hiLight){
			case 1: if(conf.speed<10) conf.speed++; break;
			case 2: if(conf.fructSize<99) conf.fructSize++; break;
			case 3: if(conf.mapSize<2) conf.mapSize++; break;
			case 4: conf.border = true; break;
			case 5: conf.teleport = true; break;
			};
				break;
	case KEY_EXIT: return; break;
	case KEY_ENTER: 
			if(hiLight==0) return;
			else if(hiLight==6) conf = {8, 1, 1, 0, 0};
			break;
	default: break;
	};
	}
}

//меню настроек управления
void Menu::ControlSettingsLoop(){
	
	int hiLight = 0;
	
	//элементы меню и названия клавиш курсора (можно было сделать через map контейнер)
	std::string menuPart[7] = {"Back", "Key Down:", "Key Up:", "Key Left:", "Key Right:", "Erase Settings"};
	std::string cursKey[4] = {"down", "up", "left", "right"};
	
	while(1){
	
	cls(menu);
	update(menu);
	
	Display::printScr(menu, MenuWidth/2 - 4, 0, (char*)"Controls", BLUE);
	
	for(int i=0; i<6;i++){
		
		if(i==hiLight) wattron(menu, A_BOLD);
		
		switch(i){
		case 1:	//определяем клавиши курсора и выводим их названия
				if(buttons.down<CURS_KEY_MIN || buttons.down>CURS_KEY_MAX)	
					Display::printScr(menu, MenuWidth-7, i+2, buttons.down);
				else Display::printScr(menu, MenuWidth-7, i+2, (char*)cursKey[buttons.down-CURS_KEY_MIN].c_str());
				break;
		case 2:
				if(buttons.up<CURS_KEY_MIN || buttons.up>CURS_KEY_MAX)
					Display::printScr(menu, MenuWidth-7, i+2, buttons.up);
				else Display::printScr(menu, MenuWidth-7, i+2, (char*)cursKey[buttons.up-CURS_KEY_MIN].c_str());
				break;
		case 3:
				if(buttons.left<CURS_KEY_MIN || buttons.left>CURS_KEY_MAX)
					Display::printScr(menu, MenuWidth-7, i+2, buttons.left);
				else Display::printScr(menu, MenuWidth-7, i+2, (char*)cursKey[buttons.left-CURS_KEY_MIN].c_str());
				break;
		case 4:
				if(buttons.right<CURS_KEY_MIN || buttons.right>CURS_KEY_MAX)
					Display::printScr(menu, MenuWidth-7, i+2, buttons.right);
				else Display::printScr(menu, MenuWidth-7, i+2, (char*)cursKey[buttons.right-CURS_KEY_MIN].c_str());
				break;
		};
		
		wattron(menu, COLOR_PAIR(GREEN));
		
		//элемент выхода из меню
		if(i==0) Display::printScr(menu, 2, 1, (char*)menuPart[0].c_str());
		//элемент очистки настроек
		else if(i==5) Display::printScr(menu, 2, MenuHeight-3, (char*)menuPart[5].c_str());
		
		else Display::printScr(menu, 2, i+2, (char*)menuPart[i].c_str());
		
		
		wattroff(menu, A_BOLD | COLOR_PAIR(GREEN));
	}
	
	switch (periph()){
	case KEY_UP: if(hiLight>0) hiLight--; break;
	case KEY_DOWN: if(hiLight<5) hiLight++; break;
	case KEY_EXIT: return; break;
	case KEY_ENTER: 
		if(hiLight==0) return;
		else if(hiLight==5) buttons = {KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN};
		else {	//создаем информационное окно для обработки нажатой клавиши
		PrintInfo(false, InfoWidth, InfoHeight-1, (char*)"Press the button!");
		
		cbreak();	//отключаем задержку приема клавиш
		int ch = getButton();	//принимаем символ
		halfdelay(1);	//включаем задержку обратно
		deleteWindow(info);	//удаляем окно
		
		switch(hiLight){	//присваиваем символ
		case 1: buttons.down = ch; break;
		case 2: buttons.up = ch; break;
		case 3: buttons.left = ch; break;
		case 4: buttons.right = ch; break;
		};
		
		} break;
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
	
	while(1){
	//вывод информации об управлении и игре
	cls(info);
	update(info);
	
	Display::printScr(info, HelpWidth - 6, 0, (char*)"HELP", BLUE);
	
	if(!select){
	Display::printScr(info, 1, 0, (char*)"(1)", BLUE);
	Display::printScr(info, 2, 1, (char*)"Controls:", YELLOW);
	wattron(info, COLOR_PAIR(GREEN));
	Display::printScr(info, 2, 2, (char*)"Key Down:");
	Display::printScr(info, 2, 3, (char*)"Key Up:");
	Display::printScr(info, 2, 4, (char*)"Key Left:");
	Display::printScr(info, 2, 5, (char*)"Key Right:");
	Display::printScr(info, 2, 6, (char*)"Key Pause Game:");
	Display::printScr(info, 2, 7, (char*)"Key Help Game:");
	Display::printScr(info, 2, 8, (char*)"Key Quit Game:");
	wattroff(info, COLOR_PAIR(GREEN));
	Display::printScr(info, HelpWidthRight, 6, 'p');
	Display::printScr(info, HelpWidthRight, 7, 'h');
	Display::printScr(info, HelpWidthRight, 8, 'q');
	
	//обработка клавиш управления
	if(buttons.down<CURS_KEY_MIN || buttons.down>CURS_KEY_MAX)
		Display::printScr(info, HelpWidthRight, 2, buttons.down);
	else Display::printScr(info, HelpWidthRight, 2, (char*)cursKey[buttons.down-CURS_KEY_MIN].c_str());
	if(buttons.up<CURS_KEY_MIN || buttons.up>CURS_KEY_MAX)
		Display::printScr(info, HelpWidthRight, 3, buttons.up);
	else Display::printScr(info, HelpWidthRight, 3, (char*)cursKey[buttons.up-CURS_KEY_MIN].c_str());
	if(buttons.left<CURS_KEY_MIN || buttons.left>CURS_KEY_MAX)
		Display::printScr(info, HelpWidthRight, 4, buttons.left);
	else Display::printScr(info, HelpWidthRight, 4, (char*)cursKey[buttons.left-CURS_KEY_MIN].c_str());
	if(buttons.right<CURS_KEY_MIN || buttons.right>CURS_KEY_MAX)
		Display::printScr(info, HelpWidthRight, 5, buttons.right);
	else Display::printScr(info, HelpWidthRight, 5, (char*)cursKey[buttons.right-CURS_KEY_MIN].c_str());
	}
	
	else {
		Display::printScr(info, 1, 0, (char*)"(2)", BLUE);
		Display::printScr(info, 2, 1, (char*)"About:", YELLOW);
		Display::printScr(info, 2, 2, (char*)"Game", GREEN);
		Display::printScr(info, 7, 2,'T' | COLOR_PAIR(GREEN));
		Display::printScr(info, 8, 2,'S' | COLOR_PAIR(GREEN));
		Display::printScr(info, 9, 2,'N' | COLOR_PAIR(YELLOW));
		Display::printScr(info, 10, 2,'A' | COLOR_PAIR(RED));
		Display::printScr(info, 11, 2,'K' | COLOR_PAIR(GREEN));
		Display::printScr(info, 12, 2,'E'| COLOR_PAIR(BLUE));
		Display::printScr(info, 14, 2, (char*)"v_1.0", BLUE);
		wattron(info, COLOR_PAIR(GREEN));
		Display::printScr(info, 2, 4, (char*)"Game created by DSuhoi (2020)");
		Display::printScr(info, 2, 5, (char*)"Email: <dsuh0i.h8@gmail.com>");
		Display::printScr(info, 2, 6, (char*)"Sourse Code:");
		Display::printScr(info, 2, 7, (char*)"https://github.com/DSuhoi/tsnake");
		Display::printScr(info, 5, 8, (char*)"Thanks for playing!");
		wattroff(info, COLOR_PAIR(GREEN));
	}
	switch(periph()){	//выход из информационного меню
	case KEY_ENTER:
	case KEY_EXIT:
	case 'h': deleteWindow(info); return; break;
	case KEY_LEFT: select = false; break;
	case KEY_RIGHT: select = true; break;
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
	
	while(1){
	
	cls(info);
	update(info);
	
	Display::printScr(info, PauseWidth - 7, 0, (char*)"PAUSE", BLUE);
	
	wattron(info, COLOR_PAIR(GREEN));
	
	for(int i=0; i<4; i++){
	if(i==hiLight) wattron(info, A_BOLD);
	
	Display::printScr(info, 2, i+1, (char*)pauseMenuStr[i].c_str());
	
	wattroff(info, A_BOLD);	
	}
	wattroff(info, COLOR_PAIR(BLUE));
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
	
	cls(info);
	update(info);
	//и выводим текст
	Display::printScr(info, InfoWidth - 6, 0, (char*)"INFO", BLUE);
	Display::printScr(info, 1, 1, buff, BLUE);
	
	if(isSelect){	//если это вопрос
		
		wattron(info, COLOR_PAIR(GREEN));
		std::string selectStr[2] = {"<No> ", "<Yes>"};
		bool hiLight = false;

	while(1){
		
		Display::printScr(info, (w/2 - 2), h-2,(char*)selectStr[hiLight].c_str());
	switch (periph()){	//возвращаем ответ
	case KEY_LEFT: hiLight = false; break;
	case KEY_RIGHT: hiLight = true; break;
	case KEY_ENTER: wattroff(info, COLOR_PAIR(GREEN)); deleteWindow(info); return hiLight; break;
	};
	} }
	
	return false;
}


CONFIG& Menu::getConfig(){ return conf; }	//возвращаем настройки карты

CONTROL& Menu::setControl(){ return buttons; } //возвращаем настройки управления
