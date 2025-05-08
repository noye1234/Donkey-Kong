#include "game.h"
#include "ghosts.h"
#include "board.h"

// Static variable to indicate if the game is running
bool Game::on = true;
bool Game::is_save=false;
bool Game::is_silence = false;
long Game::random_seed;
char Game::selected_screen;                    // Current screen selected
board Game::gameBoard;         // Current game board


// Loads the message to be displayed from a predefined array
void Game::load_message() {
    for (int i = 0; i < int(MESSAGES_LIMIT::MESSAGE_LENGTH); i++) {
        strcpy_s(message[i], int(MESSAGES_LIMIT::MESSAGE_WIDTH), create_message[i]); // Copies predefined messages into the message array
    }
}

// Erases the message by overwriting it with the saved board content
void Game::erase_message() const {
    for (int i = 0; i < int(MESSAGES_LIMIT::MESSAGE_LENGTH); i++) {
        gotoxy(int(MESSAGES_LIMIT::MESSAGE_START_X), int(MESSAGES_LIMIT::MESSAGE_START_Y) + i); // Moves cursor to message position
        for (int j = 0; j < int(MESSAGES_LIMIT::MESSAGE_WIDTH); j++) {
            std::cout << save_board[i][j]; // Outputs saved board content to erase message
        }
        std::cout << std::endl;
    }
}

// Displays the message on the screen and returns the key pressed by the user
char Game::print_message() {
    for (int i = 0; i < int(MESSAGES_LIMIT::MESSAGE_LENGTH); i++) {
        gotoxy(int(MESSAGES_LIMIT::MESSAGE_START_X), int(MESSAGES_LIMIT::MESSAGE_START_Y) + i);

        for (int j = 0; j < int(MESSAGES_LIMIT::MESSAGE_WIDTH); j++) {
            save_board[i][j] = readCharFromConsole(int(MESSAGES_LIMIT::MESSAGE_START_X) + j, int(MESSAGES_LIMIT::MESSAGE_START_Y) + i); // Reads characters from console
            std::cout << message[i][j]; // Displays message
        }
        std::cout << std::endl;
    }

    char ch = _getch(); // Waits for user input
    return ch; // Returns the pressed key
}

// Deletes the message from the screen by overwriting it with spaces
void Game::delete_message() const {
    for (int i = 0; i < int(MESSAGES_LIMIT::MESSAGE_LENGTH); i++) {
        gotoxy(int(MESSAGES_LIMIT::MESSAGE_START_X), int(MESSAGES_LIMIT::MESSAGE_START_Y) + i);
        for (int j = 0; j < int(MESSAGES_LIMIT::MESSAGE_WIDTH); j++) {
            std::cout << " "; // Overwrites message with spaces
        }
        std::cout << std::endl;
    }
}

// Loads the game's instructions and control keys from a predefined array
void Game::load_instructions_and_keys() {
    for (int i = 0; i < int(INSTRUCTIONS_AND_KEYS_LIMIT::INSTRUCTIONS_AND_KEYS_Y); i++) {
        strcpy_s(instructions_and_keys[i], int(INSTRUCTIONS_AND_KEYS_LIMIT::INSTRUCTIONS_AND_KEYS_X), create_instructions_and_keys[i]); // Copies instructions
    }
}

// Displays the game's instructions and waits for the user to press a key to continue
void Game::print_load_instructions_and_keys() const {
    system("cls"); // Clears the screen
    for (int i = 0; i < int(INSTRUCTIONS_AND_KEYS_LIMIT::INSTRUCTIONS_AND_KEYS_Y); i++) {
        gotoxy(int(INSTRUCTIONS_AND_KEYS_LIMIT::INSTRUCTIONS_AND_KEYS_START_X), int(INSTRUCTIONS_AND_KEYS_LIMIT::INSTRUCTIONS_AND_KEYS_START_Y) + i); // Positions cursor
        for (int j = 0; j < int(INSTRUCTIONS_AND_KEYS_LIMIT::INSTRUCTIONS_AND_KEYS_X); j++) {
            std::cout << instructions_and_keys[i][j]; // Displays instructions
        }
        std::cout << std::endl;
    }
    char ch = _getch(); // Waits for user input to continue
}

// Loads the game's menu from a predefined array
void Game::load_menu() {
    for (int i = 0; i < int(MENU_LIMIT::MENU_Y); i++) {
        strcpy_s(menu[i], int(MENU_LIMIT::MENU_X), create_menu[i]); // Copies menu options into the menu array
    }
}

// Displays the game's menu and returns the key pressed by the user
void Game::print_menu() const {
    clearScreen(); // Clears the screen
    for (int i = 0; i < int(MENU_LIMIT::MENU_Y); i++) {
        gotoxy(int(MENU_LIMIT::MENU_START_X), int(MENU_LIMIT::MENU_START_Y) + i); // Positions cursor for each menu item
        for (int j = 0; j < int(MENU_LIMIT::MENU_X); j++) {
            if (menu[i][j] == '*') {
                std::cout << '\\'; // Handles special character escape
            }
            else {
                std::cout << menu[i][j]; // Prints menu item
            }
        }
        std::cout << std::endl;
    }
}

// Handles the main menu logic and starts the game
char Game::start_menu() {
    //clearScreen(); // Clears screen before starting the menu
    //chooseMenuColor();  // allow the user to choose color of screen  
    clearScreen(); // Clears screen before starting the menu
    char res;
    on = true; // Sets the game state to running
    board::level_score = 0; // Resets level score
    for (int i = 0; i < boards.size(); i++)
        boards[i].set_game_score(0); // Resets game scores for each board
    print_menu(); // Displays the menu

    // Valid inputs for menu navigation
    std::vector<char> validMenuInputs = { start_game, keys_and_instruction, choose_screen, end_game };
    res = waitForValidInput(validMenuInputs); // Waits for a valid menu option

    while (res == keys_and_instruction) {
        print_load_instructions_and_keys(); // Displays instructions if selected
        print_menu(); // Displays menu again after showing instructions
        res = waitForValidInput(validMenuInputs); // Waits for valid menu input
    }

    while (res == choose_screen) {
        res = print_screen_menu(); // Displays screen selection menu
        while (res < '1' || (res > screen_name.size() + '0')) {
            std::cout << "Invalid input, please try again." << std::endl; // Validates user input
            res = _getch(); // Waits for a valid input
        }
        selected_screen = res; // Sets the selected screen
        print_menu(); // Displays menu again after selecting screen
        res = waitForValidInput(validMenuInputs); // Waits for another valid menu input
    }

    if (selected_screen < '1' || selected_screen >(screen_name.size() + '0')) {
        selected_screen = '1'; // Defaults to the first screen if input is invalid
    }

    if (res == start_game) {
        clearScreen(); // Clears screen when starting game
        if (boards.size() == 0) {
            std::cout << "error no screens found"; // Error if no screens are available
            Sleep(700); // Pauses briefly
            return try_again; // Returns to try again if no screens are available
        }
        clearScreen(); // Clears screen again before creating the game
        create_donkey_kong(); // Initializes Donkey Kong game
    }
    return res; // Returns the selected option
}


// Restarts the game state, resetting barrels, Mario, and the board
void Game::restart(board b, Mario* mario, std::vector<std::unique_ptr<hazard>>& hazard_array, int size) {
    for (auto& hazard_ : hazard_array) {
        hazard_->restart(); // Restarts all barrels
    }
    mario->restart(); // Restarts Mario's state
    b.restart(mario->get_lives()); // Resets the board

}


// Runs the game: handles setup, menu navigation, and game loop
void Game::run() {
    load_all(); // Loads all necessary game data

    while (start_menu() != end_game); // Loops until the game ends
}




// Proceeds to the next level
void Game::next_level(int& screen_index, board* my_board, Mario* mario) {
    
    screen_index = (screen_index + 1) % boards.size(); // Loops through levels
    curr_level++; // Increments level number
    on = true; // Resets game state to running
    board::level_score += (my_board->get_game_score()); // Adds game score for the level
    *my_board = boards[screen_index]; // Switches to the new level board
    clearScreen(); // Clears screen for the new level
    mario->replace(my_board->get_mariox(), my_board->get_marioy()); // Moves Mario to new position
    if (!get_silence()) {
        my_board->print_board(); // Prints the new level's board
        my_board->print_legend(mario->get_lives()); // Prints lives on the new level
    }
    my_board->set_game_score(board::level_score); // Sets score for the level
    board::level_score = 0; // Resets level score
    if (!get_silence()) my_board->print_score_and_hammer_mode(mario->get_hammer_mode()); // Prints score and hammer mode status
}


// Displays the screen selection menu and returns the key pressed by the user
char Game::print_screen_menu() const {
    system("cls"); // Clear screen (could be replaced with a more portable method)
    std::cout << "Choose a screen to start from:" << std::endl;

    // Loops through each screen name and displays it to the user
    for (int i = 0; i < screen_name.size(); i++) {
        std::cout << (i + 1) << ". " << screen_name[i] << std::endl;
    }

    // Gets user input (key pressed)
    char res = _getch();
    return res;
}

// Loads the finish screen from a predefined array
void Game::load_finish() {
    for (int i = 0; i < int(FINISH_LIMIT::FINISH_LENGTH); i++) {
        strcpy_s(finish[i], int(FINISH_LIMIT::FINISH_WIDTH), create_finish[i]); // Consider using std::string for safety
    }
}

// Displays the finish screen and waits for ESC to exit
void Game::print_finish()const {
    system("cls"); // Clear screen (could be replaced with a more portable method)
    for (int i = 0; i < int(FINISH_LIMIT::FINISH_LENGTH); i++) {
        gotoxy(int(FINISH_LIMIT::FINISH_START_X), int(FINISH_LIMIT::FINISH_START_Y) + i); // Moves cursor to specific position
        for (int j = 0; j < int(FINISH_LIMIT::FINISH_WIDTH); j++) {
            std::cout << finish[i][j];
        }
        std::cout << std::endl;
    }
    int res = _getch(); // Waits for a key press
    while (res != ESC)
        res = _getch(); // Wait for ESC input to exit
}

// Loads the win screen from a predefined array
void Game::load_win() {
    for (int i = 0; i < int(WIN_LIMIT::WIN_LENGTH); i++) {
        strcpy_s(win[i], int(WIN_LIMIT::WIN_WIDTH), create_win[i]); // Consider using std::string for safety
    }
}

// Displays the win screen and waits for ESC to exit
void Game::print_win()const {
    system("cls"); // Clear screen (could be replaced with a more portable method)
    for (int i = 0; i < int(WIN_LIMIT::WIN_LENGTH); i++) {
        gotoxy(int(WIN_LIMIT::WIN_START_X), int(WIN_LIMIT::WIN_START_Y) + i); // Moves cursor to specific position
        for (int j = 0; j < int(WIN_LIMIT::WIN_WIDTH); j++) {
            std::cout << win[i][j];
        }
        std::cout << std::endl;
    }
    gotoxy(int(WIN_LIMIT::SCORE_X), int(WIN_LIMIT::SCORE_Y)); // Move cursor for score display
    std::cout << board::level_score; // Displays the final score

    int res = _getch(); // Waits for a key press
    while (res != ESC)
        res = _getch(); // Wait for ESC input to exit
}


void Game::make_hazard(std::vector<std::unique_ptr<hazard>>& hazard_array, Mario* mario) {
    char ch;
    int count = 0;
    for (int i = START_Y; i < LENGTH; i++) {
        for (int j = START_X; j < WIDTH; j++) {
            ch = get_char(j, i);

            if (ch == barrel) {
                hazard_array.emplace_back(std::make_unique<barrels>(j, i, 1, mario));
            }

            else if (ch == ghost) {
                hazard_array.emplace_back(std::make_unique<ghosts>(j, i, mario));
            }
            else if (ch == Bghost) { 
                hazard_array.emplace_back(std::make_unique<betterGhosts>(j, i, mario));
            }
        }
    }

}
// Load game assets
void Game::load_all() {
    load_boards();
    load_message();
    load_instructions_and_keys();
    load_menu();
    load_finish();
    load_win();
}

// Gets all screen files from the current directory
void Game::get_all_screens(std::vector<std::string>& screens) {
    screens.clear();
    fs::path currentDir = fs::current_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".screen") {
                screens.push_back(entry.path().filename().string()); // Adds screen file name to the array
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n'; // Error handling for filesystem issues
    }

    if (screens.empty()) {
        std::cerr << "Error: No .screen files found in the current directory. Exiting the game." << std::endl;
        return; // Exits if no screen files are found
    }

    std::sort(screens.begin(), screens.end()); // Sorts the screens alphabetically
}

// Loads boards from the .screen files
void Game::load_boards() {
    get_all_screens(screen_files); // Retrieves all screen files
    if (screen_files.empty()) {
        std::cerr << "Error: No .screen files were provided to load. Exiting the game." << std::endl;
        return; // Exits if no .screen files are found
    }

    // Loads each board from the screen files
    for (const auto& filename : screen_files) {
        board b;
        if (!b.load(filename)) {  // If loading fails, prints an error and continues
            std::cerr << "Error: Failed to load board from file " << filename << std::endl;
            continue; // Skips this file if loading fails
        }
        screen_name.push_back(filename);
        boards.push_back(b);
    }

    if (boards.empty()) {
        std::cerr << "Error: Failed to load any boards. Exiting the game." << std::endl;
    }
}


void Game::setConsoleColor(ConsoleColor color)const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<int>(color));  //casting to int
}

void Game::chooseMenuColor()const {
    int colorChoice;
    std::cout << "Choose a color for the menu text:" << std::endl;
    std::cout << "2. Blue" << std::endl;
    std::cout << "3. Green" << std::endl;
    std::cout << "4. Cyan" << std::endl;
    std::cout << "5. Red" << std::endl;
    std::cout << "6. Magenta" << std::endl;
    std::cout << "7. Yellow" << std::endl;
    std::cout << "8. White" << std::endl;

    std::cin >> colorChoice;

    // בוחר את הצבע המבוקש מתוך האפשרויות
    ConsoleColor color;
    switch (colorChoice) {
    case 2:
        color = ConsoleColor::Blue;
        break;
    case 3:
        color = ConsoleColor::Green;
        break;
    case 4:
        color = ConsoleColor::Cyan;
        break;
    case 5:
        color = ConsoleColor::Red;
        break;
    case 6:
        color = ConsoleColor::Magenta;
        break;
    case 7:
        color = ConsoleColor::Yellow;
        break;
    case 8:
        color = ConsoleColor::White;
        break;
    default:
        std::cout << "Invalid choice, defaulting to white." << std::endl;
        Sleep(1000);
        color = ConsoleColor::White;
    }

    setConsoleColor(color);  // change the console color
}