// Include necessary headers for the game
#include "board.h"
#include "utils.h"
#include "barrels.h"
#include "mario.h"

// Determine the new direction of the barrel
void barrels::find_dir(int x, int y) {

    int tempY = y; // Temporary variable for the y-coordinate
    int tempx = x; // Temporary variable for the x-coordinate
    bool flag = true; // Flag for directional decisions
    int dirR, dirL; // Variables for right and left directions


    // Check possible directions (left and right)
    dirL = check_dir(tempx, tempY, left);
    dirR = check_dir(tempx, tempY, right);

    // If both directions are the same, set the barrel's direction
    if (dirR == dirL) {
        set_dir(dirL);
        // If the direction indicates deletion, delete the barrel
        if (get_dir() == delete_) {
            delete_barrel();
        }
    }
    // Otherwise, randomly select a direction (left or right)
    else {
        int temp = rand() % 2;

        if (temp == right) {
            set_dir(right);
        }
        else {
            set_dir(left);
        }
    }
}

// Handle barrel deletion and explosion logic
void barrels::delete_barrel() {
    fall_count = max_fall_count; // Set the fall counter to its maximum
    exploding_barrel(); // Trigger the explosion animation
}

// Manage the barrel's movement logic
void barrels::move() {
    char ch = get_char(get_x(), get_y());
    //char ch = readCharFromConsole(get_x(), get_y()); // Read character at the barrel's position
    bool flag = true;
    int res_dir = 0;

    // Check if the barrel interacts with a hammer
    if (ch == hammer_left || ch == hammer_right) {
        board::set_score(board::get_score() + hit_with_hammer); // Increase the score
        restart(); // Restart the barrel
    }
    else {
        ch = get_char(get_x(), get_y() + 1);
        //ch = readCharFromConsole(get_x(), get_y() + 1); // Check below the barrel

        // Handle interaction with floor tiles
        if (ch == middleFloor || ch == leftFloor || ch == rightFloor) {

            if (fall_count >= max_fall_count) {
                //gotoxy(get_x(), get_y());
                exploding_barrel(); // Trigger explosion
                return;
            }
            hazard::move(); // Call hazard movement logic
        }

        // Handle falling logic
        if (is_falling(ch)) {
            if (check_mario(get_x(), get_y() + 1))
                hurt_mario(); // Hurt Mario if he's under the barrel
            set_dir(stop_move); // Stop barrel movement
            fall_count++;
            move_dir(get_x(), false); // Move the barrel

            //ch = readCharFromConsole(get_x(), get_y() + 1); // Check below again
            ch = get_char(get_x(), get_y() + 1); // Check below again
            if (!is_falling(ch)) {
                fall_count++;
                find_dir(get_x(), get_y()); // Determine a new direction

                if (fall_count >= max_fall_count) {
                   // gotoxy(get_x(), get_y());
                    exploding_barrel(); // Trigger explosion if max falls reached
                    return;
                }
                fall_count = 0; // Reset fall count
            }
        }
    }
}

// Handle the barrel explosion animation
void barrels::exploding_barrel() {
    // Alternate between clearing and displaying the barrel
    if (fall_count % 2 == 1) {
        //gotoxy(get_x(), get_y());
        set_char(space,get_x(), get_y());

        //std::cout << space; // Clear the barrel
       // Sleep(5);
    }
    else {
       // gotoxy(get_x(), get_y());
         set_char(barrel, get_x(), get_y());

          // std::cout << barrel; // Display the barrel
       // Sleep(12);
    }
    fall_count++;

    // End the explosion after a certain count
    if (fall_count >= 20) {
        if (is_mario_here()) {
            hurt_mario(); // Hurt Mario if he's nearby
        }
        restart(); // Restart the barrel
    }
}

// Check if Mario is within a certain radius of the barrel
bool barrels::is_mario_here() const {
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (check_mario(get_x() + j, get_y() + i))
                return true; // Mario is nearby
        }
    }
    return false; // Mario is not nearby
}

// Reset the barrel's state
void barrels::restart() {
    hazard::restart(); // Call the base class restart logic
    fall_count = 0; // Reset the fall counter
} 
