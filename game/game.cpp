#include <iostream>
#include "../term/periph.h"
#include "../term/files.h"
#include "game.h"

// Defining Game class fields
Snake *Game::snake;
time_t Game::gameTime;
long Game::gameScore[30];

// Function for initializing components
void Game::start()
{
    Periph::initPeriph();   // Configure the periphery
    snake = nullptr;        // Null pointer
    Display::initColor();   // Configure the colors
    Menu::initMainMenu();   // Configure the main menu
    Map::initMap();         // Configure the map size
    Display::update();      // Update the window
}

// Function for configuring the field
void Game::startGame()
{   // Configure:
    Map::selectSizeMap(Menu::getConfigMap().mapSize);   // Map size
    // Creating player
    snake = new Snake();
    //Snake size
    snake->initSnake(Map::getSpawnSnake(), Map::getHeight() * Map::getWidth());
    // If you need borders on the map, create their
    if(Menu::getConfigMap().border)
        Map::initBorderCoords(snake->infoHead());
    //Number of fruits and their coordinates
    Map::initFruitCoords(Menu::getConfigMap().numFruits);
    Display::update();      // Updating screen
    gameTime = time(0);     // Save the new game time
}

// Check for fail
bool Game::checkWin()
{
    Coords snakeHead = snake->infoHead(); // Head position
    // If head doesn't cross the map border, your body or other border
    if(snakeHead.x == 0 || snakeHead.x == (Map::getWidth()) || snakeHead.y == 0 || 
    snakeHead.y == (Map::getHeight()) || Map::isSnake(snakeHead, snake->getBodyCoords(), snake->getSnakeLen())
    || Map::isBorder(snakeHead))
        return GAME_WIN;     // Then return the end game flag
    else
        return GAME_NOT_WIN; // Otherwise the game is still going on
}

// Generating the score
int Game::genScore(int level)
{
    srand(time(0));
    return level + rand()%(level + 5);
    
}

// Game process (main logic)
void Game::process()
{
    // Variable to save the game result
    int gameResult = 0;
    while(1) {
        // If you were not given the command to restart the game, go to the menu
        if(gameResult != GAME_RESTART && Menu::mainMenuLoop() == 0)
            // End of the program
            return;
        // Reset the result of the previous game
        gameResult = 0;
        
        // Configure the game
        startGame();
    
        if(Menu::getConfigMap().clearScore) {
            // If you need to erase data
            for(unsigned int i = 0; i < 30; i++)
                gameScore[i] = 0;
            FileSystem::saveRecords(gameScore); 
            Menu::getConfigMap().clearScore = false; 
        }
        
        // Reading the results of previous games
        long resultLastGame = FileSystem::loadRecords(gameScore, Menu::getConfigMap().mapSize, Menu::getConfigMap().speed);
        // Result of the current game
        long resultThisGame = 0;
        
        // Displays the static part of the menu
        Map::printSubMenuStatic(resultLastGame, Menu::getConfigMap().speed);
        
        int oldSnakeLen = snake->getSnakeLen(); // Previous length of the snake
        // Game Loop
        do {
            // Processing buttons using a custom template
            int currentButton = Periph::getButton(Menu::getControl(), static_cast<float>(10/Menu::getConfigMap().speed));
        
            switch(currentButton) {
            case 'h':   // Menu launch
                Menu::helpLoop();
                break;  
            case 'p':   // Pause
                gameResult = Menu::pauseLoop(); 
                if(gameResult == GAME_END)
                    return;
                break;
            case KEY_EXIT:  //  Exit the game
                return; 
                break;  
            case KEY_ENTER: // If the other keys are not for control, then
            case ERR:       // Moving without turns
                snake->move(snake->getVector()); 
                break;  
            default:        // Otherwise we set a new vector for the player's movement
                snake->move(currentButton); 
                break;
            };
        
            // If teleport mode is active
            if(Menu::getConfigMap().teleport) {
                Coords snakeHead = snake->infoHead();
                if(snakeHead.x == 0)
                    snake->setHeadCoords(Map::getWidth()-1, snakeHead.y);
                else if(snakeHead.x == Map::getWidth())
                    snake->setHeadCoords(1, snakeHead.y);
                else if(snakeHead.y == 0)
                    snake->setHeadCoords(snakeHead.x, Map::getHeight()-1);
                else if(snakeHead.y == Map::getHeight())
                    snake->setHeadCoords(snakeHead.x, 1);
            }
            // If the snake ate the fruit, then increase its length
            if(Map::isFruit(snake->infoHead())) {
                snake->incSnakeLen();
                Map::setFruitOnMap(snake->infoHead(), snake->getBodyCoords(), snake->getSnakeLen());
            }
            
            Map::updateMap(snake->getBodyCoords(), snake->getSnakeLen());   // Update the map
        
            if(snake->getSnakeLen()>oldSnakeLen) {
                oldSnakeLen = snake->getSnakeLen();
                resultThisGame += genScore(Menu::getConfigMap().speed);
            }
            // Output the current values of the game score, speed level, and time
            Map::printSubMenuActive(resultThisGame, gameTime);
        
        // We check the game based on the pause menu selection
        } while(checkWin() == GAME_NOT_WIN && gameResult != RETURN_MENU && gameResult != GAME_RESTART);
    
        // Kill the snake
        Map::setMap(snake->infoHead().x, snake->infoHead().y, KILL);
    
        Periph::gameDelay(20);  // Pause in 200ms
    
        if(resultLastGame < resultThisGame) {    // Save the game result
            gameScore[Menu::getConfigMap().mapSize*10 + (Menu::getConfigMap().speed-1)] = resultThisGame;
            FileSystem::saveRecords(gameScore);
        }

        // If you didn't plan to quit the game (Giving the user a choice)
        if(gameResult < GAME_RESTART && 
            Menu::printInfo(INFO_WIDTH, INFO_HEIGHT,(char*)"  Restart Game ?", true))
            gameResult = GAME_RESTART;  // If reloading the map is selected
    
        delete snake;           // Remove the snake
        snake = nullptr;
    
        Map::eraseMap();        // Erase the map
        Display::update();      // Update the window
    }
}

// End of the game
void Game::endGame()
{
    if(snake != nullptr)
        delete snake;   // Remove the player object
    Periph::erasePeriph();
}
