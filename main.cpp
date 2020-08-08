#include <iostream>
#include "game/game.h"


int main(void)
{
	Game game;		// Объект игры
	game.Start();	// Инициализация компонентов игры
	game.Process();	// Процесс игры
	game.EndGame();	// Завершение игры
	
	exit(0);
	return 0;
}
