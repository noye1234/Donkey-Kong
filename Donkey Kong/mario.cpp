#include "mario.h"


void Mario::draw() const {
    // Draw Mario's position on the screen
    position.draw();
}

void Mario::restart() {
    // Delete current Mario representation, reset position, and restart
    delete_();
    position.restart(); // Reset position
    keyPressed(start_key); // Reset key input
    position.change_hammer_mode(false); // Disable hammer mode at restart
}

void Mario::delete_() {
    // Move cursor to Mario's position and clear it
    set_char(get_backgroundChar(), get_x(), get_y());
    position.set_backgroundChar(' '); // Set the background to empty space
}

void Mario::erase() {
    // Erase Mario's current position on the screen
    position.erase();
}

void Mario::keyPressed(char key) {
    // Process the key press event
    position.KeyPressed(key);
}

Point::PublicDirection Mario::getDirection() const {
    // Get the current direction of Mario from his position
    return position.getDirection();
}

void Mario::move() {
    // Move Mario within the game boundaries (left_wall, right_wall, ceiling, bottom)
    position.move(left_wall, right_wall, ceiling, bottom);
}

char Mario::get_backgroundChar() {
    // Return the character representing Mario's background
    return position.get_backgroundChar();
}
