#include <iostream>
#include "game/game.h"


int main(void)
{
	Game game;	//объект игры
	
	game.Start();	//инициализация компонентов игры
	
	game.Process();	//процесс игры
	
	game.EndGame();	//завершение игры
	
	exit(0);
	
	return 0;
}
