#include <iostream>
#include "game/include/game.h"

int main(void)
{
    Game::start();      // Initializing game components
    Game::process();    // Game process
    Game::endGame();    // The end of the game

    return 0;
}
