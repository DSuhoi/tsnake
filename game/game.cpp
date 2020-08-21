#include <iostream>
#include "../term/periph.h"
#include "../term/files.h"
#include "game.h"

// Defining Game class fields
Snake *Game::snake;
time_t Game::GameTime;
long Game::gameScore[30];

// Function for initializing components
void Game::Start()
{
	Periph::InitPeriph();	// Configure the periphery
	snake = nullptr;		// Null pointer
	Display::InitColor();	// Configure the colors
	Menu::InitMainMenu();	// Configure the main menu
	Map::InitMap();			// Configure the map size
	Display::Update();		// Update the window
}

// Function for configuring the field
void Game::StartGame()
{	// Configure:
	Map::SelectSizeMap(Menu::GetConfigMap().mapSize);	// Map size
	
	snake = new Snake(); // Creating player
	//Snake size
	snake->InitSnake(Map::GetSpawnSnake(), Map::GetHeight() * Map::GetWidth());
	if(Menu::GetConfigMap().border){		// // If you need borders on the map, create their
		Map::InitBorderCoords(snake->InfoHead());
	}
	Map::InitFruitCoords(Menu::GetConfigMap().numFruits);	//Number of fruits and their coordinates	
	Display::Update();		// Updating screen
	GameTime = time(0);		// Save the new game time
}

// Check for fail
bool Game::CheckWin()
{
	Coords tmp = snake->InfoHead();	// Head position
	// If head doesn't cross the map border, your body or other border
	if(tmp.x == 0 || tmp.x == (Map::GetWidth()) || tmp.y == 0 || 
	tmp.y == (Map::GetHeight()) || Map::IsSnake(snake->InfoHead(),snake->GetBodyCoords(),snake->GetSnakeLen())
	|| Map::IsBorder(snake->InfoHead())){ 
		return GAME_WIN;	// Then return the end game flag
	}
	else{ 
		return GAME_NOT_WIN;	// Otherwise the game is still going on
	}
}

// Generating the score
int Game::GenScore(int level)
{
	srand(time(0));
	return level + rand()%(level + 5);
	
}

// Game process (main logic)
void Game::Process()
{
	// Variable to save the game result
	int gameResult = 0;
	while(1){
		// If you were not given the command to restart the game, go to the menu
		if(gameResult != GAME_RESTART && Menu::MainMenuLoop() == 0){
			// End of the program
			return;
		}
		// Reset the result of the previous game
		gameResult = 0;
		
		// Configure the game
		StartGame();
	
		if(Menu::GetConfigMap().clearScore){
			// If you need to erase data
			for(int i = 0; i < 30; i++){
				gameScore[i] = 0;
			}	
			FileSystem::SaveRecords(gameScore); 
			Menu::GetConfigMap().clearScore = false; 
		}
		
		// Reading the results of previous games
		long resultLastGame = FileSystem::LoadRecords(gameScore, Menu::GetConfigMap().mapSize, Menu::GetConfigMap().speed);
		// Result of the current game
		long resultThisGame = 0;
		
		// Displays the static part of the menu
		Map::PrintSubMenuStatic(resultLastGame, Menu::GetConfigMap().speed);
		
		int oldSnakeLen = snake->GetSnakeLen();	// Previous length of the snake
		// Game Loop
		do{
			// Processing buttons using a custom template
			int currentButton = Periph::GetButton(Menu::GetControl(), static_cast<float>(10/Menu::GetConfigMap().speed));
		
			switch (currentButton){
			case 'h':	// Menu launch
				Menu::HelpLoop();
				break;	
			case 'p':	// Pause
				gameResult = Menu::PauseLoop(); 
				if(gameResult == GAME_END){
					return;
				}
				break;
			case KEY_EXIT:	//  Exit the game
				return; 
				break;	
			case KEY_ENTER:	// If the other keys are not for control, then
			case ERR:	// Moving without turns
				snake->Move(snake->GetVector()); 
				break;	
			default:	// Otherwise we set a new vector for the player's movement
				snake->Move(currentButton); 
				break;
			};
		
			// If teleport mode is active
			if(Menu::GetConfigMap().teleport){
				if(snake->InfoHead().x == 0){ 
					snake->SetHeadCoords(Map::GetWidth()-1, snake->InfoHead().y);
				}
				else if(snake->InfoHead().x == Map::GetWidth()){
					snake->SetHeadCoords(1, snake->InfoHead().y);
				}
				else if(snake->InfoHead().y == 0){ 
					snake->SetHeadCoords(snake->InfoHead().x, Map::GetHeight()-1);
				}
				else if(snake->InfoHead().y == Map::GetHeight()){ 
					snake->SetHeadCoords(snake->InfoHead().x, 1);
				}
			}
			
			if(Map::IsFruit(snake->InfoHead())){	// If the snake ate the fruit, then increase its length
				snake->IncSnakeLen();
				Map::SetFruitOnMap(snake->InfoHead(), snake->GetBodyCoords(), snake->GetSnakeLen());
			}
			
			Map::UpdateMap(snake->GetBodyCoords(), snake->GetSnakeLen());	// Update the map
		
			if(snake->GetSnakeLen()>oldSnakeLen){
				oldSnakeLen = snake->GetSnakeLen();
				resultThisGame += GenScore(Menu::GetConfigMap().speed);
			}
			// Output the current values of the game score, speed level, and time
			Map::PrintSubMenuActive(resultThisGame, GameTime);
		
		// We check the game based on the pause menu selection
		}while(CheckWin() == GAME_NOT_WIN && gameResult != RETURN_MENU && gameResult != GAME_RESTART);
	
		// Kill the snake
		Map::SetMap(snake->InfoHead().x, snake->InfoHead().y, KILL);
	
		Periph::GameDelay(20);	// Pause in 200ms
	
		if(resultLastGame < resultThisGame){	// Save the game result
			gameScore[Menu::GetConfigMap().mapSize*10 + (Menu::GetConfigMap().speed-1)] = resultThisGame;
			FileSystem::SaveRecords(gameScore);
		}

		// If you didn't plan to quit the game (lose)
		if(gameResult < GAME_RESTART){	// Giving the user a choice
			if(Menu::PrintInfo(INFO_WIDTH, INFO_HEIGHT,(char*)"  Restart Game ?", true)){
				gameResult = GAME_RESTART;	// If reloading the map is selected
			}
		}
	
		delete snake;			// Remove the snake
		snake = nullptr;
	
		Map::EraseMap();		// Erase the map
		Display::Update();		// Update the window
	}
}

// End of the game
void Game::EndGame()
{
	if(snake != nullptr){ 
		delete snake;	// Remove the player object
	}
	Periph::ErasePeriph();
}
