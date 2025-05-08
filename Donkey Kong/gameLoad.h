#pragma once
#pragma once
#include "library.h"
#include "mario.h"
#include "barrels.h"
#include "ghosts.h"
#include "betterGhosts.h"
//NEW:
#include "Steps.h"
#include "Results.h"



// Game class definition
class GameLoad: public Game {
    // Member variables
    //std::vector<board> boards;                // Stores all boards
  //  std::vector<std::string> screen_files;   // Files corresponding to each screen
    //std::vector<std::string> screen_name;    // Names of the screens
    char selected_screen;                    // Current screen selected


    //char instructions_and_keys[LENGTH][WIDTH]; // Stores the instructions and keys
    //const char* create_instructions_and_keys[DEFAULT_WIDTH] = {
    //    "         Oh no! Donkey Kong               ",
    //    "     has kidnapped Princess Pauline!      ",
    //    "      Mario, only you can save her,       ",
    //    "         but it won't be easy...          ",
    //    "     Donkey Kong will throw barrels       ",
    //    "           that can kill you.             ",
    //    "   Good luck Mario Paulin trusts you!     ",
    //    "------------------------------------------",
    //    "|These are the keys                      |",
    //    "|you will use in the game:               |",
    //    "------------------------------------------",
    //    "|   LEFT                        a or A   |",
    //    "------------------------------------------",
    //    "|   RIGHT                       d or D   |",
    //    "------------------------------------------",
    //    "|   UP / JUMP                   w or W   |",
    //    "------------------------------------------",
    //    "|   DOWN                        x or X   |",
    //    "------------------------------------------",
    //    "|   STAY                        s or S   |",
    //    "------------------------------------------",
    //};

    //char save_board[LENGTH][WIDTH];            // Temporary save for the current board
    //char message[LENGTH][WIDTH];               // Stores pause messages
    //const char* create_message[DEFAULT_WIDTH] = {
    //    "------------------------------------------",
    //    "|Game paused, press ESC again to continue|",
    //    "------------------------------------------",
    //};

    //char menu[LENGTH][WIDTH];                  // Stores the menu
    //const char* create_menu[DEFAULT_WIDTH] = {
    //    " _____   ____  _   _ _  __________     __ ",
    //    "|  __ * / __ *| * | | |/ /  ____* *   / / ",
    //    "| |  | | |  | |  *| | ' /| |__   * *_/ /  ",
    //    "| |  | | |  | | . ` |  < |  __|   *   /   ",
    //    "| |__| | |__| | |*  | . *| |____   | |    ",
    //    "|_____/ *____/|_| *_|_|*_*______|  |_|    ",
    //    "        _  ______  _   _  _____           ",
    //    "       | |/ / __ *| * | |/ ____|          ",
    //    "       | ' / |  | |  *| | |  __           ",
    //    "       |  <| |  | | . ` | | |_ |          ",
    //    "       | . * |__| | |*  | |__| |          ",
    //    "       |_|*_*____/|_| *_|*_____|          ",
    //    "----------------------------------------- ",
    //    "|(1) Start a new game                   | ",
    //    "----------------------------------------- ",
    //    "|(7) Choose starting screen             | ",
    //    "----------------------------------------- ",
    //    "|(8) Present instructions and keys      | ",
    //    "----------------------------------------- ",
    //    "|(9) EXIT                               | ",
    //    "----------------------------------------- ",
    //};

    //char finish[LENGTH][WIDTH];                 // Game over message
    //const char* create_finish[DEFAULT_WIDTH] = {
    //    "------------------------------------------",
    //    "|              GAME OVER                 |",
    //    "|         -------------------            |",
    //    "|              TRY AGAIN!                |",
    //    "|                                        |",
    //    "|   -- press esc to go back to menu --   |",
    //    "------------------------------------------",
    //};

    //char win[LENGTH][WIDTH];                    // Victory screen
    //const char* create_win[DEFAULT_WIDTH] = {
    //    "------------------------------------------",
    //    "|               YOU WON!                 |",
    //    "|                                        |",
    //    "|       YOUR SCORE IS:                   |",
    //    "|                                        |",
    //    "|   -- press esc to go back to menu --   |",
    //    "------------------------------------------",
    //};

    int max_level = 3;       // Maximum levels in the game
    int curr_level = 1;      // Current level
   // static bool on;          // Tracks if the game is running
    board gameBoard;         // Current game board



    std::vector<std::string> fileNames;
    bool esc_pressed = false;
    bool unmatching_result_found = false;
    std::string filename_prefix;
    std::string stepsFilename;
    std::string resultsFilename ;
    long random_seed;

    Steps steps;
    Results results;

    size_t iteration = 0; // we need iteration to be outside the loop
    std::vector<std::string> screens;
public:
    // Static methods to manage game state
    // static bool is_on() { return on; }
    // static void change_on(bool to_change) {  = to_change; }

    // Methods to manage game assets
    void get_all_screens(std::vector<std::string>& screens);
    //void load_boards();

    void run(); // Starts the game loop
    

    // Load game assets
    //void load_all() {
    //    load_boards();
    //    load_message();
    //    load_instructions_and_keys();
    //    load_menu();
    //    load_finish();
    //    load_win();
    //}

    //// Menu management
    //void load_menu();
    //void print_menu() const;
    //char start_menu();

    //// Instructions and keys
    //void load_instructions_and_keys();
    //void print_load_instructions_and_keys() const;

    //// Pause message
    //void load_message();
    //char print_message();
    //void erase_message() const;
    //void delete_message() const;

    //// Finish screen
    //void load_finish();
    //void print_finish() const;

    //// Win screen
    //void load_win();
    //void print_win() const;

    //// Game mechanics
    //void restart(board b, Mario* mario, std::vector<barrels>& barrel, int size, std::vector<ghosts>& ghost_array);
    void create_donkey_kong();
    //board& my_board, Mario& mario, std::vector<barrels>& barrels_array, std::vector<ghosts>& ghost_array, std::vector<betterGhosts>& betterGhost_array, bool isLoad, bool isSave, Steps& steps, Results& results
    // 
    // 
    //void make_better_ghosts(std::vector<betterGhosts>& ghost_array, Mario* mario);
    //void make_ghosts(std::vector<ghosts>& ghost_array, Mario* mario);
    //void make_barrels(std::vector<barrels>& barrels_array, Mario* mario);
    //char print_screen_menu() const;

    //void find_hazard(std::vector<ghosts>& ghost_array_, std::vector<barrels>& barrels_array_, Mario* mario_);
    //void next_level(int& screen_index, board* my_board, Mario* mario);





   // void get_all_screens(std::vector<std::string>& screens);
    void report_result_error(const std::string& message, const std::string& filename, size_t iteration);

};
