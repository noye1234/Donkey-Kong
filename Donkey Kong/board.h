#pragma once
#include "library.h"
#include "utils.h"
#include "point.h"

// Constants for the board's starting position, dimensions, and symbols
constexpr int START_Y = 1;
constexpr int LENGTH = 25;
constexpr int START_X = 1;
constexpr int WIDTH = 80;

// Symbols used in the game
constexpr char ladder_symbol = 'H'; // Symbol representing a ladder
constexpr char mario = '@';         // Symbol representing Mario
constexpr char barrel = 'O';        // Symbol representing a barrel
constexpr char ghost = 'x';         // Symbol representing a ghost
constexpr char Bghost = 'X';         // Symbol representing a ghost
constexpr char princess = '$';      // Symbol representing the princess
constexpr char donkeyKong = '&';    // Symbol representing Donkey Kong
constexpr char rightFloor = '>';    // Symbol for the floor sloping to the right
constexpr char leftFloor = '<';     // Symbol for the floor sloping to the left
constexpr char middleFloor = '=';   // Symbol for the flat floor
constexpr char wall = 'Q';          // Symbol for a wall
constexpr char space = ' ';         // Symbol for empty space

// Score changes for various actions in the game
constexpr char jumpe_above_ghost = 60; // Score for jumping over a ghost
constexpr char jumpe_above_barrel = 50; // Score for jumping over a barrel
constexpr char hit_with_hammer = 30;    // Score for hitting with a hammer 

// Initial number of lives for Mario
constexpr int LIVES = 3;



class board {

    // Mario's starting position
    int mario_x = 32;
    int mario_y = 2;

    // Princess's position
    int princess_x = 32;
    int princess_y = 6;

    // Coordinates for life and score display
    int LIFE_X = 2;
    int LIFE_Y = 1;
    int score_X = 1;
    int score_Y = 1;

    int curr_life = 1; // Current life count


    friend class Game;
    friend class GameLoad;


    static int level_score; // Static variable to track the score of the current level
    int score = 0;
    bool board_hammer_mode = false; // Indicates whether hammer mode is active

    char marioSymbol = '@';


    // The board state is represented by two 2D character arrays
    char currentBoard[LENGTH][WIDTH + 1]; // Current state of the board

    char create_board[LENGTH][WIDTH + 1]; // Initial configuration of the board
    
    //NEW2
    //std::chrono::time_point<std::chrono::steady_clock> start_time;
    static std::chrono::time_point<std::chrono::steady_clock> start_time;
    bool is_fast_mode = false;

    //NEW
    Point startPos{0, 0 };
    Point player{ 0, 0 };
    Point originalEndPos{ 0, 0 };
    Point endPos{ 0, 0 };
    bool isSilent = false;
public:
    // Static methods to get and set the score
    static int get_score() { return level_score; }
    static void set_score(int new_score) { level_score = new_score; }

    // Getter and setter for the game score
    void set_game_score(int score_) { score = score_; }
    int get_game_score() { return score; }

    // Methods to manage and display the board
    void print_board(); // Prints the current state of the board
    void print_legend(const int num) const; // Prints the game legend
    void restart(const int num) {
        level_score = 0;
        if (!get_silence()) {
            print_board();
            print_score_and_hammer_mode(board_hammer_mode);
            print_legend(num);
        }
    } // Restarts the game

    void print_score_and_hammer_mode(bool hammer_mode); // Prints the score and hammer mode status
    void loadBoard(); // Loads the board's configuration

    // Getter methods for Mario's position
    int get_mariox() { return mario_x; }
    int get_marioy() { return mario_y; }

    // Method to load a board configuration from a file
    bool load(const std::string& filename);

    // Getter and setter for Mario's symbol
    char getMarioSymbol() const { return marioSymbol; }
    void setMarioSymbol(char symbol) { marioSymbol = symbol; }
    void chooseMarioSymbol(); // Allows the user to choose Mario's symbol






    void setIsSilent(bool isSilentMode) {
        isSilent = isSilentMode;
    }
    //void reset();
    //void print() const;
    char getChar(int x, int y) const {
        return currentBoard[y][x];
    }
    char getChar(Point p) const {
        return getChar(p.x, p.y);
    }
    bool finished() const {
        return player == endPos;
    }
    bool keyPressed(char key) {
         return player.KeyPressed(key);
    }
    void erasePlayer() {
        player.erase();
    }
    void drawPlayer() const {
        if (!isSilent) player.draw();
    }
    

    void movePlayer() {
         player.move(START_X, WIDTH,START_Y, LENGTH);
    }
    void randomlyChangeEndPoint();

    //new2
    char get_curr_char(int i, int j) { 
        return create_board[j][i]; 
    }
    void set_curr_char(char ch,int i, int j) {
        create_board[j][i]=ch;
    }
    void initializeTimer(bool fastMode);
    void updateAndPrintTimer() const;
    static int getElapsedTime();
};