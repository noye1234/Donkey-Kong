#pragma once

#include "library.h"
#include "board.h"
#include "point.h"

// The maximum number of lives Mario has
//constexpr int LIVES = 3;
constexpr char start_key = 's';
class Mario {
    // Enum for game boundaries
    enum { left_wall = START_X, right_wall = WIDTH - 3, ceiling = START_Y, bottom = LENGTH - 2 };
    Point position;// Represents Mario's position on the board

    char marioSymbol;

public:
    void change_is_injured(bool res) { position.change_is_injured(res); }// change "is_injured" value
    bool get_is_injured() { return position.get_is_injured(); }  //get "is_injured" value

    int get_lives() { return position.get_lives(); };
    void set_lives(const int _lives) { position.set_lives(_lives); };



    // Constructor to initialize Mario at a starting position
    Mario(int startX, int startY, char symbol = '@') : position(startX, startY, symbol), marioSymbol(symbol) { };

    // Draws Mario's current position on the board
    void draw() const;

    // Restarts Mario's position to the starting coordinates
    void restart();

    // Erases Mario's character from the console, leaving the background character
    void delete_();

    int get_x() const { return position.getX(); }    // Gets Mario's X-coordinate
    int get_y() const { return position.getY(); }  // Gets Mario's Y-coordinate
    void set_start(int x_, int y_) { position.set_start(x_, y_); }
    void set_x(int x_) { position.setX(x_); }
    void set_y(int y_) { position.setX(y_); }

    // Erases Mario's current position
    void erase();

    // Handles keypresses to update Mario's direction or actions
    void keyPressed(char key);

    // Retrieves Mario's current direction as a public struct
    Point::PublicDirection getDirection() const;

    // Moves Mario based on his current direction, within the game boundaries
    void move();

    // Gets the background character at Mario's current position
    char get_backgroundChar();


    void setMarioSymbol(char symbol) { marioSymbol = symbol; }
    char getMarioSymbol() const { return marioSymbol; }

    bool get_hammer_mode() { return position.get_hammer_mode(); }
    void erase_hammer() { position.erase_hammer(); }
    void replace(int x_, int y_) { set_start(x_, y_); restart(); }
    void remove_lives() { position.remove_live(); }



    //new
    bool is_key_change() { return position.key_change(); }
    void set_key_change(bool change) { position.set_key_change(change); }
    char get_marioKey() { return position.get_mario_key(); }

    char get_char(int x, int y) { position.get_char(x, y); }
    void set_char(char ch,int x, int y) { position.set_char(ch,x, y); }
}; 