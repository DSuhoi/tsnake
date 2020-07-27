#include <ncurses.h>
#include "../menu/menu.h"
#include "periph.h"
#include <unistd.h> 


// Метод-обёртка ручной задержки
void Periph::GameDelay(int delayTime)
{ 
	usleep(delayTime * SLEEP_TIME); 
}

// Иницилизация экрана и обработки клавиш
void Periph::InitPeriph()
{
	initscr();
	start_color();
	noecho();
	//halfdelay(1);
	nodelay(stdscr,true);
	keypad(stdscr,true);
	curs_set(0);
}

// Завершение функций обработки клавиш
void Periph::ErasePeriph()
{
	keypad(stdscr,false);
	cbreak();
	endwin();
}

// Обработка кнопок
int Periph::GetButton()
{
	
	usleep(100000);	// Задержка для приёма
	int button = getch();
	flushinp();		// Очищаем буфер клавиатуры
	int selectButton = ERR;
	switch(button){
	case KEY_DOWN: 
		selectButton = KEY_DOWN; 
		break;
	case KEY_UP: 
		selectButton = KEY_UP; 
		break;
	case KEY_LEFT: 
		selectButton = KEY_LEFT; 
		break;
	case KEY_RIGHT: 
		selectButton = KEY_RIGHT; 
		break;
	case 10: 
		selectButton = KEY_ENTER; 
		break;
	case 27: 
	case 'q': 
	case 'Q': 
		selectButton = KEY_EXIT; 
		break;
	case 'h': 
	case 'H': 
		selectButton = 'h'; 
		break;
	case 'p': 
	case 'P': 
		selectButton = 'p'; 
		break;
	default: 
		selectButton = ERR; 
		break;
	};
	return selectButton;
}


// Обработка игровых кнопок
int Periph::GetButton(int *gameButtons, float pauseDelay)
{
	usleep(pauseDelay*30000);
	int button = getch();
	flushinp();
	int selectButton = ERR;
	if(button == gameButtons[DOWN]){ 
		selectButton = KEY_DOWN;
	}
	else if(button == gameButtons[UP]){ 
		selectButton = KEY_UP;
	}
	else if(button == gameButtons[LEFT]){ 
		selectButton = KEY_LEFT;
	}
	else if(button == gameButtons[RIGHT]){ 
		selectButton = KEY_RIGHT;
	}
	else{ 
		switch(button){
		case 10: 
			selectButton = KEY_ENTER; 
			break;
		case 27: 
		case 'q': 
		case 'Q': 
			selectButton = KEY_EXIT; 
			break;
		case 'h': 
		case 'H': 
			selectButton = 'h'; 
			break;
		case 'p': 
		case 'P': 
			selectButton = 'p'; 
			break;
		default: 
			selectButton = ERR; 
			break;
		};
	}	
	return selectButton;
}
