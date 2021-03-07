#include "game.hpp"

int main()
{
    srand(time(nullptr));

    Game::start();      // Initializing game components
    Game::process();    // Game process
    Game::end_game();   // The end of the game

    return 0;
}
