#include <iostream>
#include "game/game.h"


int main(void)
{
	Game::Start();		// Initializing game components
	Game::Process();	// Game process
	Game::EndGame();	// The end of the game

	return 0;
}
