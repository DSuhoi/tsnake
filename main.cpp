#include "game/game.h"

int main(void)
{	
	Game game;	//объект игры
	
	game.Start();	//инициализация компонентов игры
	
	game.process();	//процесс игры
	
	return 0;
}
