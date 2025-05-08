#include "gameSave.h"
#include "ghosts.h"
#include "board.h"



// Main game loop, creating Donkey Kong, barrels, and Mario
void GameSave::create_donkey_kong() {
    bool is_finish = false; // Game loop flag for finish
    board my_board; // Board object to store the current game board

    std::vector<std::string> screens;
    get_all_screens(screens);
    if (get_save()) {
        for (int i = 0; i < screens.size(); i++) {
            filename_prefix = screens[i].substr(0, screens[i].find_last_of('.'));
            stepsFilename = filename_prefix + ".steps";
            resultsFilename = filename_prefix + ".result";

            std::ofstream stepsFile(stepsFilename, std::ios::trunc);
            std::ofstream resultsFile(resultsFilename, std::ios::trunc);
        }
    }
     // Load selected screen
    int screen_index = get_selected_screen() - '1'; // Determines the screen index from the selected screen

    if (screen_index >= 0 && screen_index < get_boards_size()) {
        my_board = get_curr_board(screen_index);

        setBoard(my_board);

        filename_prefix = screens[screen_index].substr(0, screens[screen_index].find_last_of('.'));
    }
    else {
        my_board = get_curr_board(0);// Defaults to the first screen if the index is out of range
        setBoard(my_board);

        filename_prefix = screens[0].substr(0, screens[0].find_last_of('.'));

    }

    //
    stepsFilename = filename_prefix + ".steps";
    resultsFilename = filename_prefix + ".result";

    if (get_save()) {
        
        set_random_seed(static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count()));
        steps.setRandomSeed(get_random_seed());
        srand(get_random_seed()); // Seeds random number generation for randomness
    }
    

    clearScreen(); // Clears the screen before displaying the board
    my_board.print_board(); // Prints the board
    my_board.print_legend(LIVES); // Prints legend with the number of lives
    my_board.print_score_and_hammer_mode(false); // Displays score and hammer mode status

    char temp_key, key = start_key; // Initializes key variables
    Mario mario(my_board.get_mariox(), my_board.get_marioy(), gameBoard.getMarioSymbol()); // Initializes Mario at the given position
    mario.set_lives(LIVES); // Sets Mario's lives
    int temp_lives = mario.get_lives(), temp_score = my_board.get_score();

    //new2
    std::vector<std::unique_ptr<hazard>> hazard_array;
    make_hazard(hazard_array, &mario);
    int size_hazard_array = hazard_array.size();

    // Game loop
    size_t iteration = 0;
    while (mario.get_lives() > 0 && !is_finish) {

        if (!Game::is_on()) {
            if (curr_level < max_level) {
                curr_level++;
                if (get_save()) {

                    results.addResult(iteration, Results::finished);
                    steps.saveSteps(stepsFilename);
                    results.saveResults(resultsFilename);
                }
                
                next_level(screen_index, &my_board, &mario); // Moves to next level
                setBoard(my_board);

                filename_prefix = screens[screen_index].substr(0, screens[screen_index].find_last_of('.'));
                stepsFilename = filename_prefix + ".steps";
                resultsFilename = filename_prefix + ".result";

                if (get_save()) 
                   steps.setRandomSeed(get_random_seed());

                hazard_array.clear();
                make_hazard(hazard_array, &mario);
                size_hazard_array = hazard_array.size();
                iteration = 0;
            }
            else {
                break; // Ends game if max level is reached
            }
        }
        if (mario.get_is_injured() || temp_lives > mario.get_lives() || !Game::is_on()) {
            results.addResult(iteration, Results::hitBomb);

            restart(my_board, &mario, hazard_array, size_hazard_array); // Restarts game state
            setBoard(my_board);

            mario.change_is_injured(false); // Resets Mario's injury status
            temp_lives = mario.get_lives(); // Updates lives
        }

        for (int i = 0; i < size_hazard_array; i++) {
            hazard_array[i]->move(); // Moves each ghost

            Sleep(10);

        }

        mario.draw(); // Draws Mario on the screen
        while (_kbhit()) {
            temp_key = _getch(); // Captures any key pressed
            if (temp_key == ESC) {
                temp_key = print_message(); // Pauses game if ESC is pressed
                if (temp_key == ESC) {
                    delete_message(); // Resumes the game after pausing
                    erase_message();
                    getBoard().print_legend(LIVES); // Prints the board legend
                }
                else {
                    if(get_save())
                      steps.addStep(iteration, ESC);
                    is_finish = true; // Ends game if user exits
                    break;
                }
            }
            else
                key = temp_key; // Updates key if another key is pressed
            mario.keyPressed(temp_key); // Processes key press
            if (mario.is_key_change() && get_save()) {
                steps.addStep(iteration, temp_key);
                mario.set_key_change(false);
            }
        }

        mario.move(); // Moves Mario based on input
        Sleep(15);


        getBoard().print_score_and_hammer_mode(mario.get_hammer_mode()); // Updates score and hammer mode status
        ++iteration;

    }

    if (is_finish) {
        results.addResult(iteration, Results::hitBomb);
    }
    if (get_save()) {
        results.addResult(iteration, Results::finished);
        steps.saveSteps(stepsFilename);
        results.saveResults(resultsFilename);
    }
    iteration = 0;

    if (!Game::is_on()) {
        my_board.set_score(my_board.get_game_score()+ my_board.get_score()); // Adds the game score if finished
        print_win(); // Displays win message
    }
    else
        print_finish(); // Displays finish message if game ends prematurely

}


