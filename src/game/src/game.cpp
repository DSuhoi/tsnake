#include "game.hpp"

// Defining Game class fields
Snake *Game::snake;
time_t Game::game_time;
long Game::game_score[30];

// Function for initializing components
void Game::start()
{
    Periph::init_periph();   // Configure the periphery
    snake = nullptr;        // Null pointer
    Display::init_color();   // Configure the colors
    Menu::init_main_menu();   // Configure the main menu
    Map::init_map();         // Configure the map size
    Display::update();      // Update the window
}

// Function for configuring the field
void Game::start_game()
{   // Configure:
    Map::select_size_map(Menu::get_config_map().map_size);   // Map size
    // Creating player
    snake = new Snake(Map::get_spawn_snake());
    // If you need borders on the map, create their
    if (Menu::get_config_map().border)
        Map::init_border_coords(snake->info_head());
    //Number of fruits and their coordinates
    Map::init_fruit_coords(Menu::get_config_map().num_fruits);
    Display::update();      // Updating screen
    game_time = time(0);     // Save the new game time
}

// Check for fail
bool Game::check_win()
{
    Coords snake_head = snake->info_head(); // Head position
    // If head doesn't cross the map border, your body or other border
    if (snake_head.x == 0 || snake_head.x == (Map::get_width()) || snake_head.y == 0 || 
    snake_head.y == (Map::get_height()) || Map::is_snake(snake_head, snake->get_body_coords(), snake->get_snake_len())
    || Map::is_border(snake_head))
        return GAME_WIN;     // Then return the end game flag
    else
        return GAME_NOT_WIN; // Otherwise the game is still going on
}

// Generating the score
int Game::gen_score(int level)
{
    srand(time(0));
    return level + rand()%(level + 5);  
}

// Game process (main logic)
void Game::process()
{
    // Variable to save the game result
    int game_result = 0;
    while (1) {
        // If you were not given the command to restart the game, go to the menu
        if (game_result != GAME_RESTART && Menu::main_menu_loop() == 0)
            // End of the program
            return;
        // Reset the result of the previous game
        game_result = 0;
        
        // Configure the game
        start_game();
    
        if (Menu::get_config_map().clear_score) {
            // If you need to erase data
            for (unsigned int i = 0; i < 30; ++i)
                game_score[i] = 0;
            FileSystem::save_records(game_score); 
            Menu::get_config_map().clear_score = false; 
        }
        
        // Reading the results of previous games
        long result_last_game = FileSystem::load_records(game_score, Menu::get_config_map().map_size, Menu::get_config_map().speed);
        // Result of the current game
        long result_this_game = 0;
        
        // Displays the static part of the menu
        Map::print_sub_menu_static(result_last_game, Menu::get_config_map().speed);
        
        int old_snake_len = snake->get_snake_len(); // Previous length of the snake
        // Game Loop
        do {
            // Processing buttons using a custom template
            int current_button = Periph::get_button(Menu::get_control(), static_cast<float>(10/Menu::get_config_map().speed));
        
            switch(current_button) {
            case 'h':   // Menu launch
                Menu::help_loop();
                break;  
            case 'p':   // Pause
                game_result = Menu::pause_loop(); 
                if(game_result == GAME_END)
                    return;
                break;
            case KEY_EXIT:  //  Exit the game
                return; 
                break;  
            case KEY_ENTER: // If the other keys are not for control, then
            case ERR:       // Moving without turns
                snake->move(snake->get_vector()); 
                break;  
            default:        // Otherwise we set a new vector for the player's movement
                snake->move(current_button); 
                break;
            };
        
            // If teleport mode is active
            if (Menu::get_config_map().teleport) {
                Coords snake_head = snake->info_head();
                if (snake_head.x == 0)
                    snake->set_head_coords(Map::get_width()-1, snake_head.y);
                else if (snake_head.x == Map::get_width())
                    snake->set_head_coords(1, snake_head.y);
                else if (snake_head.y == 0)
                    snake->set_head_coords(snake_head.x, Map::get_height()-1);
                else if (snake_head.y == Map::get_height())
                    snake->set_head_coords(snake_head.x, 1);
            }
            // If the snake ate the fruit, then increase its length
            if (Map::is_fruit(snake->info_head())) {
                snake->inc_snake_len();
                Map::set_fruit_on_map(snake->info_head(), snake->get_body_coords(), snake->get_snake_len());
            }
            
            Map::update_map(snake->get_body_coords(), snake->get_snake_len());   // Update the map
        
            if (snake->get_snake_len() > old_snake_len) {
                old_snake_len = snake->get_snake_len();
                result_this_game += gen_score(Menu::get_config_map().speed);
            }
            // Output the current values of the game score, speed level, and time
            Map::print_sub_menu_active(result_this_game, game_time);
        
        // We check the game based on the pause menu selection
        } while(check_win() == GAME_NOT_WIN && game_result != RETURN_MENU && game_result != GAME_RESTART);
    
        // Kill the snake
        Map::set_map(snake->info_head().x, snake->info_head().y, KILL);
    
        Periph::game_delay(20);  // Pause in 200ms
    
        if (result_last_game < result_this_game) {    // Save the game result
            game_score[Menu::get_config_map().map_size*10 + (Menu::get_config_map().speed - 1)] = result_this_game;
            FileSystem::save_records(game_score);
        }

        // If you didn't plan to quit the game (Giving the user a choice)
        if (game_result < GAME_RESTART && 
            Menu::print_info(INFO_WIDTH, INFO_HEIGHT,(char*)"  Restart Game ?", true))
            game_result = GAME_RESTART;  // If reloading the map is selected
    
        delete snake;           // Remove the snake
        snake = nullptr;
    
        Map::erase_map();        // Erase the map
        Display::update();      // Update the window
    }
}

// End of the game
void Game::end_game()
{
    if (snake != nullptr)
        delete snake;   // Remove the player object
    Periph::erase_periph();
}
