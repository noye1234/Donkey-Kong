#include "point.h"
#include "mario.h"
#include "Barrels.h"
#include "ghosts.h"


#include "game.h"




#define H 72

char Point::get_char(int x, int y) const { return Game::get_char(x, y); }
void Point::set_char(char ch, int x, int y) { Game::set_char(ch, x, y); }

// Draws the point's character with a specific color
void  Point::drawWithColor(char c, int color) const {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	Game::set_char(c, x, y);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
// This function handles keyboard input and updates Mario's direction or state based on the pressed key.
bool Point::KeyPressed(char key) {
	if (key == ESC) {
		old_key = key;
		return true;
	}
	for (size_t i = 0; i < numKeys; i++) {
		if (std::tolower(key) == keys[i]) {
			if ((key == 'w' || key == 'W') && get_char(x, y - 1) == ladder_symbol) {
				// Mario is on a ladder and wants to climb up
				dir = { 0, -1 };

				is_key_change = true;

				old_key = key;

				return true;

			}
			else if ((key == 'w' || key == 'W') && backgroundChar == ladder_symbol && get_char(x, y - 1) == '=') {
				// Mario is on a ladder and wants to climb up
				dir = { 0, -1 };

				is_key_change = true;
				old_key = key;

				return true;

			}
			else if ((key == 'x' || key == 'X') && get_char(x, y + 1) == ladder_symbol) {
				// Mario is on a ladder and wants to climb down
				dir = { 0, 1 };

				is_key_change = true;

				old_key = key;

				return true;

			}
			else if ((key == 'x' || key == 'X') && get_char(x, y + 1) == '=' && get_char(x, y + 2) == ladder_symbol) {
				//Mario is on a ladder and wants to climb down
				dir = { 0, 1 };

				is_key_change = true;

				old_key = key;
				return true;

			}
			// Handle jumping logic
			else if ((key == 'w' || key == 'W') && !jumping) {
				// Regular jump
				jumping = true;
				jumpPhase = 1;
				jumpDir = dir;


				is_key_change = true;

				old_key = key;

				return true;

			}
			else {

				if (key == 'd' || key == 'D')
					dir.x = 1;
				else if (key == 'a' || key == 'A')
					dir.x = -1;
				else if (key == 's' || key == 'S')
					dir = { 0,0 }; // Regular movement
				
				if (key != old_key && is_save()) {
					is_key_change = true;
				}
			    old_key = key;

				if (key != 's' && key != 'S') {
					hammer.dir = dir.x;

					if (key != old_key && is_save()) {
						is_key_change = true;
					}

					old_key = key;

					return true;

				}
				
			}
			break;
		}
	}
	if ((key == 'p' || key == 'P') && (hammer.hammer_mode))
	{

		punch_mode = true;

		if (is_save()) {
			is_key_change = true;
		}
		old_key = key;

		return true;
	}
	return false;

}

void Point::start_punch() {
	hammer.is_restart = false;

	if (dir.x == 0 || get_char(x + dir.x, y) == wall) {
		hammer.x1 = x + hammer.dir;
		hammer.x2 = x + 2 * hammer.dir;
	}
	else {
		hammer.x1 = x + 2 * hammer.dir;
		hammer.x2 = x + 3 * hammer.dir;
	}
	if (hammer.dir < 0)
		hammer.symbol = hammer_left;
	else
		hammer.symbol = hammer_right;



	hammer.y1 = hammer.y2 = y;
	if (hammer.x1 > 0) {
		hammer.backgroundChar1 = get_char(hammer.x1, hammer.y1);
		if (hammer.x2 > 0)
			hammer.backgroundChar2 = get_char(hammer.x2, hammer.y2);
		hammer_blow();
		hammer.is_deleted = false;
	}
	else
		hammer.is_deleted = true;

}

void Point::erase_hammer() {
	if (hammer.hammer_mode && !hammer.is_restart && !hammer.is_deleted)
	{
		if (hammer.backgroundChar1 != barrel && hammer.backgroundChar1 != ghost && hammer.backgroundChar1 != Bghost && hammer.backgroundChar1 != wall && hammer.backgroundChar1 != ch) {
			set_char(hammer.backgroundChar1, hammer.x1, hammer.y1);

		}
		if (hammer.backgroundChar2 != barrel && hammer.backgroundChar2 != ghost && hammer.backgroundChar2 != Bghost && hammer.backgroundChar1 != wall && hammer.x2 > 0 && hammer.backgroundChar1 != ch) {
			set_char(hammer.backgroundChar2, hammer.x2, hammer.y2);

		}
	}
	hammer.is_deleted = true;
}

void Point::hammer_blow() {
	if (hammer.backgroundChar1 != wall) {
		if (get_char(hammer.x1, hammer.y1) == barrel || get_char(hammer.x2, hammer.y2) == ghost)
			add_points(hit_with_hammer);

		if (!is_it_special_char(get_char(hammer.x1, hammer.y1))) {

			set_char(hammer.symbol, hammer.x1, hammer.y1);
		}

		if (!is_it_special_char(get_char(hammer.x2, hammer.y2)) && hammer.backgroundChar2 != wall) {

			set_char(hammer.symbol, hammer.x2, hammer.y2);
		}
	}
	else
		hammer.is_deleted = true;

}

// This function erases Mario from his current position by drawing the background character.
void Point::erase() {

	set_char(backgroundChar, x, y);
}

// This function updates Mario's position based on movement rules, interaction with ladders, platforms, and gravity.
void Point::move(int minX, int maxX, int minY, int maxY) {
	erase_hammer();
	if (punch_mode)
	{
		start_punch();
		punch_mode = false;
	}
	char aboveChar = wall;
	char above2Char = wall;
	// Restore the character where Mario was
	char temp_backgroundChar = backgroundChar;
	// Check if Mario has reached the princess and end the game.
	if (backgroundChar == princess) {
		off();
		return;
	}

	// Check if Mario is injured
	if (is_injured(get_char(x + dir.x, y)))
		return;

	set_char(backgroundChar, x, y);


	// Update the background character for the new position
	update_backgroundChar();

	// Horizontal movement
	int newX = x + dir.x;
	if (newX >= minX && newX <= maxX && get_char(newX, y) != wall) {
		x = newX;
	}

	// Read the characters around Mario to determine his interactions
	char currentChar = get_char(x, y);
	char belowChar = get_char(x, y + 1);
	char below2Char = get_char(x, y + 2);
	if (y - 2 >= minY) {
		aboveChar = get_char(x, y - 1);
		above2Char = get_char(x, y - 2);
	}

	// Check if Mario touches the hammer ('p')
	if (currentChar == hammer_right) {
		change_hammer_mode(true); // Activate hammer mode
	}

	// To see if Mario will land on a barrel
	if (is_injured(belowChar))
		return;

	// Handle ladder interactions
	if (currentChar == ladder_symbol) {
		fall_count = 0;
		if (dir.y == -1) { // Climbing up
			if (aboveChar == ladder_symbol) {
				y -= 1; // Move up on the ladder
			}
			else if (aboveChar == middleFloor) {
				y -= 2; // Move through the '=' to the next platform
				backgroundChar = get_char(x, y); // Update background character after move
				dir.y = 0;
				jumping = false;
			}
		}
		else if (dir.y == 1 && belowChar == ladder_symbol)
			y += 1; // Move down the ladder

		dir.y = 0; // Reset vertical direction after climbing
	}
	else if (belowChar == middleFloor && below2Char == ladder_symbol && dir.y == 1) {
		// Allow Mario to descend through '=' onto the ladder
		y += 2;
		backgroundChar = get_char(x, y); // Update background character after move
		dir.y = 0;
	}
	else if (jumping) {
		// Jump logic
		if (jumpPhase == 1) {
			// Check if there's a platform above Mario's head
			if ((aboveChar == middleFloor || aboveChar == leftFloor || aboveChar == rightFloor || aboveChar == wall) && currentChar != ladder_symbol) {
				// If there's a platform above, stop the jump
				jumping = false;
				jumpPhase = 0;
			}
			else if (above2Char == middleFloor || above2Char == leftFloor || above2Char == rightFloor || above2Char == wall)
			{
				y -= 1; // Jump up
				y = max(minY, y); // Ensure Mario doesn't go above the ceiling
				jumpPhase = 2; // Proceed to next phase
			}
			else {
				y -= 2; // Jump up
				y = max(minY, y); // Ensure Mario doesn't go above the ceiling
				jumpPhase = 2; // Proceed to next phase
			}

		}
		else if (jumpPhase == 2) {
			if (!is_falling(belowChar)) {
				// Mario lands on a platform
				jumping = false;
				fall_count--;
			}
			else if (y + 1 <= maxY) {
				fall_count++;

				//y += 2; // Continue falling within bounds
				y += 1; // Continue falling within bounds
			}
			else {
				y = maxY; // Stop at bottom
				jumping = false;
			}
		}
		check_barrel();
		check_ghost();
	}
	else if (is_falling(belowChar)) {
		fall_count++;
		// Gravity: Apply if not jumping and not on a ladder
		if (y + 1 <= maxY) {
			y += 1; // Gravity pulls Mario down
		}
		else {
			check_barrel();
			check_ghost();
		}
		//check_barrel();
		is_injured(belowChar);

	}
	if (is_injured(currentChar))
		return;

	// Handle falling logic
	if (is_done_falling()) {

		if (fall_count >= 5 || belowChar == wall) {
			remove_live();
		}
		restart_fall_count();
	}

	// Update the background character
	update_backgroundChar();
	// Draw Mario at the new position
	if (backgroundChar == hammer_right || backgroundChar == hammer_left)
		backgroundChar = temp_backgroundChar;
	//new2
	if(!get_silence())
	   draw();

}

// This function checks if Mario is injured and handles the consequences if true.
bool Point::is_injured(const char currentChar) {
	if (currentChar == barrel || currentChar == ghost || is_mario_injured || currentChar == Bghost) {
		x = x - dir.x;// Revert position
		y = y - dir.y;
		remove_live();
		return true;
	}
	return false;
}

// This function updates the background character at Mario's position.
void Point::update_backgroundChar() {

	backgroundChar = get_char(x, y);

	if (backgroundChar == barrel) {
		backgroundChar = space;// Clear the barrel
	}
}

// This function checks for barrels near Mario and adds points according to that.
void Point::check_barrel()const {
	if (get_char(x, y + 1) == barrel || get_char(x, y + 2) == barrel || get_char(x - dir.x, y) == barrel
		|| get_char(x, y) == barrel || get_char(x - dir.x, y + 2) == barrel || get_char(x - dir.x, y + 1) == barrel) {
		add_points(jumpe_above_barrel);
	}
}
void Point::check_ghost()const {
	if (get_char(x, y + 1) == ghost || get_char(x, y + 2) == ghost || get_char(x - dir.x, y) == ghost
		|| get_char(x, y) == ghost || get_char(x - dir.x, y + 2) == ghost || get_char(x - dir.x, y + 1) == ghost) {
		add_points(jumpe_above_ghost);
	}
}
// This function adds a specified number of points to the score.
void Point::add_points(const int num) const {
	board::set_score(board::get_score() + num);
}

// This function checks if Mario has stopped falling.
bool Point::is_done_falling() const {
	return (backgroundChar == space && (get_char(x, y + 1) == middleFloor ||
		get_char(x, y + 1) == rightFloor || get_char(x, y + 1) == leftFloor));
}

//Decreases Mario's life count by 1.
void Point::remove_live() {
	lives--;
} 