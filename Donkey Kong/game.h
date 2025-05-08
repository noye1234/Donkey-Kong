#pragma once
#include "library.h"
#include "mario.h"
#include "barrels.h"
#include "ghosts.h"
#include "betterGhosts.h"



#include "Steps.h"
#include "Results.h"
// Game Control Options
constexpr char start_game = '1';        // Start a new game
constexpr char keys_and_instruction = '8'; // Display keys and instructions
constexpr char end_game = '9';         // Exit the game
constexpr char choose_screen = '7';    // Choose a starting screen
constexpr char try_again = '0';        // Try again after game over

// Constants
constexpr int DEFAULT_WIDTH = 44;      // Default width for messages and menus
constexpr int ESC = 27;                // ASCII code for the Escape key

// Layout and dimensions for various screens
enum class MESSAGES_LIMIT {
    MESSAGE_WIDTH = 43,
    MESSAGE_LENGTH = 3,
    MESSAGE_START_X = 12,
    MESSAGE_START_Y = 10
};

enum class INSTRUCTIONS_AND_KEYS_LIMIT {
    INSTRUCTIONS_AND_KEYS_X = 43,
    INSTRUCTIONS_AND_KEYS_Y = 21,
    INSTRUCTIONS_AND_KEYS_START_X = 12,
    INSTRUCTIONS_AND_KEYS_START_Y = 1
};

enum class MENU_LIMIT {
    MENU_X = 43,
    MENU_Y = 21,
    MENU_START_X = 12,
    MENU_START_Y = 1
};

enum class FINISH_LIMIT {
    FINISH_WIDTH = 43,
    FINISH_LENGTH = 7,
    FINISH_START_X = 12,
    FINISH_START_Y = 5
};

enum class WIN_LIMIT {
    WIN_WIDTH = 43,
    WIN_LENGTH = 7,
    WIN_START_X = 12,
    WIN_START_Y = 5,
    SCORE_X = 36,
    SCORE_Y = 8
};

// Options for selecting game screens
enum ScreenOptions {
    screen_1 = '1', // First screen
    screen_2 = '2', // Second screen
    screen_3 = '3'  // Third screen
};

//color for the print
enum class ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Yellow = 6,
    White = 7
};

// Game class definition
class Game {


    // Member variables
    std::vector<board> boards;                // Stores all boards
    std::vector<std::string> screen_files;   // Files corresponding to each screen
    std::vector<std::string> screen_name;    // Names of the screens
    static char selected_screen;                    // Current screen selected

    char instructions_and_keys[LENGTH][WIDTH]; // Stores the instructions and keys
    const char* create_instructions_and_keys[DEFAULT_WIDTH] = {
        "         Oh no! Donkey Kong               ",
        "     has kidnapped Princess Pauline!      ",
        "      Mario, only you can save her,       ",
        "         but it won't be easy...          ",
        "     Donkey Kong will throw barrels       ",
        "           that can kill you.             ",
        "   Good luck Mario Paulin trusts you!     ",
        "------------------------------------------",
        "|These are the keys                      |",
        "|you will use in the game:               |",
        "------------------------------------------",
        "|   LEFT                        a or A   |",
        "------------------------------------------",
        "|   RIGHT                       d or D   |",
        "------------------------------------------",
        "|   UP / JUMP                   w or W   |",
        "------------------------------------------",
        "|   DOWN                        x or X   |",
        "------------------------------------------",
        "|   STAY                        s or S   |",
        "------------------------------------------",
    };

    char save_board[LENGTH][WIDTH];            // Temporary save for the current board
    char message[LENGTH][WIDTH];               // Stores pause messages
    const char* create_message[DEFAULT_WIDTH] = {
        "------------------------------------------",
        "|Game paused, press ESC again to continue|",
        "------------------------------------------",
    };

    char menu[LENGTH][WIDTH];                  // Stores the menu
    const char* create_menu[DEFAULT_WIDTH] = {
        " _____   ____  _   _ _  __________     __ ",
        "|  __ * / __ *| * | | |/ /  ____* *   / / ",
        "| |  | | |  | |  *| | ' /| |__   * *_/ /  ",
        "| |  | | |  | | . ` |  < |  __|   *   /   ",
        "| |__| | |__| | |*  | . *| |____   | |    ",
        "|_____/ *____/|_| *_|_|*_*______|  |_|    ",
        "        _  ______  _   _  _____           ",
        "       | |/ / __ *| * | |/ ____|          ",
        "       | ' / |  | |  *| | |  __           ",
        "       |  <| |  | | . ` | | |_ |          ",
        "       | . * |__| | |*  | |__| |          ",
        "       |_|*_*____/|_| *_|*_____|          ",
        "----------------------------------------- ",
        "|(1) Start a new game                   | ",
        "----------------------------------------- ",
        "|(7) Choose starting screen             | ",
        "----------------------------------------- ",
        "|(8) Present instructions and keys      | ",
        "----------------------------------------- ",
        "|(9) EXIT                               | ",
        "----------------------------------------- ",
    };

    char finish[LENGTH][WIDTH];                 // Game over message
    const char* create_finish[DEFAULT_WIDTH] = {
        "------------------------------------------",
        "|              GAME OVER                 |",
        "|         -------------------            |",
        "|              TRY AGAIN!                |",
        "|                                        |",
        "|   -- press esc to go back to menu --   |",
        "------------------------------------------",
    };

    char win[LENGTH][WIDTH];                    // Victory screen
    const char* create_win[DEFAULT_WIDTH] = {
        "------------------------------------------",
        "|               YOU WON!                 |",
        "|                                        |",
        "|       YOUR SCORE IS:                   |",
        "|                                        |",
        "|   -- press esc to go back to menu --   |",
        "------------------------------------------",
    };




    int max_level = 3;       // Maximum levels in the game
    int curr_level = 1;      // Current level
    static bool on;          // Tracks if the game is running

    static board gameBoard;         // Current game board


    Steps steps;
    Results results;
    std::string filename_prefix;
    std::string stepsFilename;
    std::string resultsFilename;
    static long random_seed;
    static bool is_silence;
    static bool is_save;

public:
    void setBoard(board b) { gameBoard = b; }
    board& getBoard() { return gameBoard; }

    void set_silence(bool choice) { is_silence = choice; }
    static bool get_silence() { return is_silence; }
    int get_curr_level() { return curr_level; }

    void set_save(bool choice) { is_save = choice; }
    static bool get_save() { return is_save; }

    void make_hazard(std::vector<std::unique_ptr<hazard>>& hazard_array, Mario* mario);

    static long get_random_seed() { return random_seed; }
    void set_random_seed(long random) { random_seed=random; }
    static char get_selected_screen() { return selected_screen; }

    // Static methods to manage game state
    static bool is_on() { return on; }
    static void change_on(bool to_change) { on = to_change; }
    static bool get_is_save() {  return is_save; }

    
    // Methods to manage game assets
    virtual void get_all_screens(std::vector<std::string>& screens);
    virtual void load_boards();

    virtual void run(); // Starts the game loop

    // Load game assets
    void load_all();

    // Menu management
    void load_menu();
    void print_menu() const;
    char start_menu();

    // Instructions and keys
    void load_instructions_and_keys();
    void print_load_instructions_and_keys() const;

    // Pause message
    void load_message();
    char print_message();
    void erase_message() const;
    void delete_message() const;

    // Finish screen
    void load_finish();
    void print_finish() const;

    // Win screen
    void load_win();
    void print_win() const;

    // Game mechanics
    void restart(board b, Mario* mario, std::vector<std::unique_ptr<hazard>>& hazard_array, int size);
    virtual void create_donkey_kong() {};


    char print_screen_menu() const;

    virtual void next_level(int& screen_index, board* my_board, Mario* mario);

    board get_curr_board(int index) { return boards[index]; }
    int get_boards_size() { return boards.size(); }


    static char get_char(int i, int j) { 
        return gameBoard.get_curr_char(i, j); 
    }
    static void set_char(char ch,int i, int j) {
        gameBoard.set_curr_char(ch,i, j); 
        if (!is_silence)
        { gotoxy(i, j); 
        std::cout << ch; };
    }

    void setConsoleColor(ConsoleColor color)const;
    void chooseMenuColor()const;


    virtual ~Game() {};
}; 
