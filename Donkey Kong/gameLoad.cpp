#include "game.h"
#include "ghosts.h"
#include "board.h"
#include "gameLoad.h"



// Runs the game: handles setup, menu navigation, and game loop
void GameLoad::run() {
    ShowConsoleCursor(false);
    load_all();

    create_donkey_kong();
    
}


// Main game loop, creating Donkey Kong, barrels, and Mario
void GameLoad::create_donkey_kong() {
    bool is_finish_ = false; // Game loop flag for finish
    board my_board; // Board object to store the current game board

    std::vector<std::string> screens;
    get_all_screens(screens);


    int screen_index = steps.get_selected_screen() - '1'; // Determines the screen index from the selected screen

    if (screen_index >= 0 && screen_index < get_boards_size()) {

        my_board = get_curr_board(screen_index);
        filename_prefix = screens[screen_index].substr(0, screens[screen_index].find_last_of('.'));


    }
    else {

        my_board = get_curr_board(0);// Defaults to the first screen if the index is out of range

        filename_prefix = screens[0].substr(0, screens[0].find_last_of('.'));

    }
    setBoard(my_board);

    stepsFilename = filename_prefix + ".steps";
    resultsFilename = filename_prefix + ".result";

    steps = steps.loadSteps(stepsFilename);
    
    if (steps.is_empty())
    {
        std::cout << "error1";
        return;
    }
    random_seed = steps.getRandomSeed();
    results = Results::loadResults(resultsFilename);
    if (results.is_empty())
    {
        std::cout << "error1";
        return;
    }
    srand(random_seed);
    
    Mario mario(my_board.get_mariox(), my_board.get_marioy(), gameBoard.getMarioSymbol());
    mario.set_lives(LIVES);

    std::vector<betterGhosts> betterGhost_array;

    clearScreen(); // Clears the screen before displaying the board
    if (!get_silence()) {
        my_board.print_board(); // Prints the board
        my_board.print_legend(LIVES); // Prints legend with the number of lives
        my_board.print_score_and_hammer_mode(false); // Displays score and hammer mode status
    }
    char temp_key, key = start_key; // Initializes key variables
    mario.set_lives(LIVES); // Sets Mario's lives
    int temp_lives = mario.get_lives(), temp_score = my_board.level_score;


    std::vector<std::unique_ptr<hazard>> hazard_array;
    make_hazard(hazard_array, &mario);
    int size_hazard_array = hazard_array.size();

    size_t iteration = 0; // Counter for iterations
    size_t nextBombIteration = 0; // Tracks the next bomb 

    // Game loop
    for (; mario.get_lives() > 0 && !is_finish_;   ++iteration ) {

        // Update hazards for next level if necessary
        if (!Game::is_on()) {
            if (curr_level < max_level) {
                curr_level++;
                next_level(screen_index, &my_board, &mario); // Moves to next level

                filename_prefix = screens[screen_index].substr(0, screens[screen_index].find_last_of('.'));
                stepsFilename = filename_prefix + ".steps";
                resultsFilename = filename_prefix + ".result";

                steps = steps.loadSteps(stepsFilename);

                results = Results::loadResults(resultsFilename);

                setBoard(my_board);
                hazard_array.clear();
                make_hazard(hazard_array, &mario);
                size_hazard_array = hazard_array.size();

                nextBombIteration = 0;
                iteration = 0;
            }
            else {
                break; // Ends game if max level is reached
            }
        }

        if (results.isFinishedBy(iteration) && get_silence()) {
            report_result_error("Results file reached finish while game hadn't!", resultsFilename, iteration);
            is_finish_ = true;
            break;
        }
        else {
            nextBombIteration = results.getNextBombIteration();
        }

        // Restart level if Mario is injured or game reset
        if (mario.get_is_injured() || temp_lives > mario.get_lives() || !Game::is_on()) {

            restart(my_board, &mario, hazard_array, size_hazard_array); // Restarts game state
            setBoard(my_board);

            mario.change_is_injured(false); // Resets Mario's injury status
            temp_lives = mario.get_lives(); // Updates lives

            if ((results.popResult() != std::pair{ iteration, Results::hitBomb })&& (get_silence())) {
                report_result_error("Results file doesn't match hit bomb event!", resultsFilename, iteration);
                unmatching_result_found = true;
                break;
            }
            nextBombIteration = results.getNextBombIteration();
        }


        for (int i = 0; i < hazard_array.size(); i++) {
            hazard_array[i]->move(); // Moves each ghost


        }
        if (!get_silence()) {
            // Draw Mario
            mario.draw();
        }
        if (!get_silence())
            Sleep(30);
            // Load next step from file
        while (steps.isNextStepOnIteration(iteration)) {
            mario.keyPressed(steps.popStep());
            if (mario.get_marioKey() == ESC ) {
                is_finish_ = true;
            }
        }
        mario.move();


     
        if (get_silence()) {
            // Check if the current iteration matches the next bomb iteration
            if (iteration == nextBombIteration && iteration > 0) {
                report_result_error("Results file has a hit bomb event that didn't happen!", resultsFilename, iteration);
                is_finish_ = true;
                break;
            }

        }
        else
            getBoard().print_score_and_hammer_mode(mario.get_hammer_mode()); // Updates score and hammer mode status

    }
    if (get_silence()) {
        // new cheack of game result
        if (results.popResult() != std::pair{ iteration, Results::finished }) {
            report_result_error("Results file doesn't match finished event!", resultsFilename, iteration);
        }
        if (results.popResult().second != Results::noResult) {
            report_result_error("Results file has additional events after finish event!", resultsFilename, iteration);
        }
    }
    else {
        if (!Game::is_on()) {
            my_board.set_score(my_board.get_game_score() + my_board.get_score()); // Adds the game score if finished
            print_win(); // Displays win message
        }
        else
            print_finish(); // Displays finish message if game ends prematurely
    }
}




void GameLoad::get_all_screens(std::vector<std::string>& screens) {
    screens.clear();
    namespace fs = std::filesystem;
    fs::path currentDir = fs::current_path();

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".screen") {
                screens.push_back(entry.path().filename().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }

    if (screens.empty()) {
        std::cerr << "Error: No .screen files found in the current directory. Exiting the game." << std::endl;
    }

    std::sort(screens.begin(), screens.end());
}

void GameLoad::report_result_error(const std::string& message, const std::string& filename, size_t iteration) {
    system("cls");
    std::cout << "Screen " << filename << " - " << message << '\n';
    std::cout << "Iteration: " << iteration << '\n';
    std::cout << "Press any key to continue to next screens (if any)" << std::endl;
    _getch();
}



