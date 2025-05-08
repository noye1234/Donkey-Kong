#include "utils.h"
#include "mario.h"
#include "hazard.h"


#include "game.h"



void hazard::restart() {

    set_char(backgroundchar,x,y);
    is_dead = false; // Reset dead status
    symbol = save_symbol; // Restore original symbol
    x = start_x; // Reset position to start_x
    y = start_y; // Reset position to start_y
    dir = start_dir; // Reset direction to start_dir
    backgroundchar=space;

}

void hazard::dissapir() {

    set_char(backgroundchar, x, y);
    symbol = space; // Change symbol to space (removes hazard from the board)
    is_dead = true; // Set the hazard as dead
}

// Check if Mario has hit the barrel with the hammer
bool hazard::check_hammer()
{
    //char ch = readCharFromConsole(x, y);
    char ch = get_char(x, y);

    // Check if Mario has hit the barrel with the hammer
    if ((ch == hammer_right || ch == hammer_left) && mario->get_hammer_mode()) {
        board::set_score(board::get_score() + hit_with_hammer); // Increase score for hitting with hammer
        dissapir(); // Remove the hazard
        return true;
    }
    return false;
}

int hazard::get_mario_x()
{
    return mario->get_x();
}

int hazard::get_mario_y()
{
    return mario->get_y();
}

bool hazard::good_backgroundchar()
{

    if (backgroundchar != barrel && backgroundchar != Bghost && backgroundchar != ghost && backgroundchar != hammer_right && backgroundchar != mario->getMarioSymbol())
       return true;

    return false;
}

void hazard::hurt_mario() {
    // If the hazard is not dead, hurt Mario
    if (!is_dead)
        mario->change_is_injured(true);
}



// Handle barrel movement logic
void hazard::move() {
    // If the hazard is dead, do nothing
    if (is_dead)
        return;

    char ch =get_char(x,y);

    if (!check_hammer()){

        // If the background character is not a barrel or ghost, reset the background
        if (good_backgroundchar())
            set_char(backgroundchar, x, y);

        // Move the barrel in the current direction
        if (x + dir<WIDTH && x + dir>START_X)
            x = x + dir;
        else
            dir = -dir;
        if (!check_hammer()) {
            ch = get_char(x, y);
            backgroundchar = ch; // Update the background character
            move_dir(x, (is_there_ladder(x, y))); // Move the barrel based on ladder presence
        }
    }
}



// Move the barrel in the given direction
void hazard::move_dir(int x_, bool is_there_ladder) {
    if (check_mario_dir()) // Check if Mario is in the barrel's path
        return;

    // If there is a ladder or the barrel is near a hammer, move in the current direction
    if (is_there_ladder || (hammer_right == backgroundchar && !mario->get_hammer_mode()) ||
        princess == backgroundchar || donkeyKong == backgroundchar) {

        set_char(space, x_ - dir, y);
        set_char(symbol, x_, y);

        set_char(backgroundchar, x_, y);

        x += dir; // Move hazard
        backgroundchar = get_char(x, y);

        // If Mario is in the hazard's path, stop movement
        if (check_mario_dir())
            return;

        set_char(symbol, x_ + dir, y);

    }
    else {

        if (is_falling(get_char(x_, y + 1))) {
           // gotoxy(x_, y);
            if (good_backgroundchar())
                set_char(backgroundchar, x_, y);

            else
                set_char(space, x_, y);

            y++; // Move down

            set_char(symbol, x_, y);

        }
        else {
            set_char(symbol, x_, y);
        }
    }
}

// Check if Mario is in the hazard's current position
bool hazard::check_mario_dir() {
    // If Mario is at the current or next position, hurt Mario
    if (((x == mario->get_x() && y == mario->get_y()) || (x == mario->get_x() && (y + 1) == mario->get_y())) && !is_dead) {
        x -= dir; // Move hazard back
        hurt_mario(); // Hurt Mario
        return true;
    }
    return false;
}

// Verify if Mario is in a specific position relative to the barrel
bool hazard::check_mario(const int _x, const int _y) const {
    if (_x == mario->get_x() && _y == mario->get_y()) // Check if Mario is at the specified position
        return true;
    return false;
}

// Check the barrel's current direction
int hazard::check_dir(int x, int y, int temp_dir) {
    bool flag = true;
    int res_dir;
    char ch, ch2;

    ch = get_char(x, (y + 1));    // Check beneath the barrel
    if (ch == wall || get_char(x, (y + 2)) == wall) {
        return delete_; // If there's a wall, delete the barrel
    }

    // If the floor is to the right or left, set direction accordingly
    if (ch == rightFloor) {
        return right;
    }
    if (ch == leftFloor) {
        return left;
    }

    x = x + temp_dir;

    // Continue checking in the current direction
    while (flag) {
        ch = get_char(x, y + 1);
        ch2 = get_char(x, y);

        // If floor is detected, set the barrel direction
        if (ch == rightFloor) {
            res_dir = right;
            flag = false;
        }
        else if (ch == leftFloor) {
            res_dir = left;
            flag = false;
        }
        // If empty space or wall, reverse direction
        else if (ch == space || ch == wall)
        {
            res_dir = -temp_dir;
            flag = false;
        }
        else if (ch2 == donkeyKong || ch2 == princess)
        {
            res_dir = -temp_dir;
            flag = false;
        }
        else {
            x = x + temp_dir; // Keep moving in the current direction
        }
    }
    return res_dir; // Return the determined direction
}
 
char hazard::get_char(int x, int y) { return Game::get_char(x, y); }
void hazard::set_char(char ch, int x, int y) { Game::set_char(ch, x, y); }