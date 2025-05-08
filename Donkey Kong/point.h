#pragma once

//#include "board.h"
#include "utils.h"
#include "library.h"


//#include "game.h"

constexpr char hammer_right = 'p';
constexpr char hammer_left = 'q';

class board;
class game;

// Class representing a point in a game (e.g., Mario's position and movement).
class Point {
public:
    // Struct for exposing direction publicly
    struct PublicDirection {
        int x, y;
    };

    struct Hammer {
        char symbol = 'p';
        char backgroundChar1 = ' ', backgroundChar2 = ' ';
        int x1, y1, x2, y2;
        int dir = 1;
        bool hammer_mode = false;
        bool is_restart = true;
        bool is_deleted = false;
    };
private:
    //NEW:
    board* pBoard = nullptr;
    friend class board;

    char old_key='s';
    bool is_key_change=true;


    // Array of valid keys for movement
    static constexpr char keys[] = { 'w','W', 'a','A', 'x','X', 'd','D', 's','S' };

    static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);
    // Struct for handling internal direction
    struct Direction { int x, y; };          // Move up         Move left       Move down     Move right    Stop movement
    static constexpr Direction direction[] = { {0, -1},{0, -1},{-1, 0},{-1, 0},{0, 1},{0, 1},{1, 0},{1, 0},{0, 0},{0, 0} };

    // Position of the point (e.g., Mario's position)
    int x, y;
    int start_x, start_y;
    int lives; // Mario's remaining lives   (there's only one Mario so it's not that bad, Sarel approved this for us)
    bool is_mario_injured; // Indicates if Mario is currently injured  (there's only one Mario so it's not that bad, Sarel approved this for us)
    Direction dir{ 0, 0 };    // Current direction of movement

    Direction jumpDir{ 0, 0 };    // Direction used during jumping

    char ch = '*';
    bool jumping = false;
    bool ladder = false;
    int jumpPhase = 0;

    void draw(char c) const {
        gotoxy(x, y);
        std::cout << c;
    }
    char backgroundChar = ' ';
    int fall_count = 0;    // Counter for falling logic

    Hammer hammer;
    bool punch_mode = false;

public:
    //NEW:
    void setBoard(board& board) {
        pBoard = &board;
    }
    bool key_change() { return is_key_change; }
    void set_key_change(bool choice) { is_key_change = choice; }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }


    // Constructor to initialize the point with a starting position and character              //
    Point(int startX, int startY, char character='@') : x(startX), y(startY), ch(character), start_x(startX), start_y(startY) { }

    // Draws the point's character at its current position
    void draw() const {
        draw(ch);
    }

    // Draws the point's character with a specific color
    void drawWithColor(char c, int color) const;

    void erase();
    bool KeyPressed(char key);
    void move(int minX, int maxX, int minY, int maxY);

    int getX() const { return x; }    // Getter for x-coordinate
    int getY() const { return y; }    // Getter for y-coordinate

    void setX(const int newX) { x = newX; }    // Setter for x-coordinate
    void setY(const int newY) { y = newY; }    // Setter for y-coordinate
    void set_start(const int newX, const int newY) { start_x = newX; start_y = newY; }    // Setter for x-coordinate


    void change_is_injured(bool res) { is_mario_injured = res; }// change "is_injured" value
    bool get_is_injured() { return is_mario_injured; }  //get "is_injured" value
    int get_lives() { return lives; };
    void set_lives(const int _lives) { lives = _lives; };

    // Gets the current direction as a public struct
    PublicDirection getDirection() const {
        return PublicDirection{ dir.x, dir.y };
    }
    // Sets the background character for the point's current position
    void set_backgroundChar(const char new_char) { backgroundChar = new_char; }
    void add_points(const int num) const;
    void check_barrel()const;
    void check_ghost()const;

    void restart_fall_count() { fall_count = 0; }    // Resets the fall counter
    bool is_done_falling() const;
    char get_backgroundChar() { return backgroundChar; }    // Getter for the background character
    void update_backgroundChar();
    bool is_injured(const char currentChar);

    bool get_hammer_mode() { return hammer.hammer_mode; }
    void hammer_blow();
    void erase_hammer();
    void start_punch();
    void change_hammer_mode(bool change) { hammer.hammer_mode = change; }
    void restart() { x = start_x; y = start_y; }

    void remove_live();


    char get_mario_key() { return old_key; }
    char get_char(int x, int y) const;
    void set_char(char ch, int x, int y);
};


