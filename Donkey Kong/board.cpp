#include "utils.h"
#include "board.h"
#include "game.h"

// Initialize the static member variable for score
// This variable keeps track of the level-specific score.
int board::level_score = 0;
std::chrono::time_point<std::chrono::steady_clock> board::start_time;

// Update and display the current score on the board, along with hammer mode status
void board::print_score_and_hammer_mode(bool hammer_mode) {
    board_hammer_mode = hammer_mode; // Set the hammer mode status
    gotoxy(score_X, score_Y); // Move the cursor to the score display position
    std::cout << std::string(4, ' '); // Clear previous score display
    gotoxy(score_X, score_Y); // Reset cursor position for score display
    std::cout << "score: " << (level_score + score) << " hammer_mode: "; // Display updated score
    if (hammer_mode)
        std::cout << "on "; // Show hammer mode as "on"
    else
        std::cout << "off"; // Show hammer mode as "off"
    updateAndPrintTimer();
}

// Print the entire game board to the console
void board::print_board() {
    gotoxy(0, 0); // Move the cursor to the top-left corner of the console
    loadBoard(); // Load the board layout
    for (int i = 0; i < LENGTH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            std::cout << currentBoard[i][j]; // Print each character on the board
        }
        std::cout << std::endl; // Move to the next line after printing a row
    }
    initializeTimer(Game::get_silence);
}

// Load the board layout from a predefined 2D array
void board::loadBoard() {
    for (int i = 0; i < LENGTH; i++) {
        strncpy_s(currentBoard[i], create_board[i], WIDTH); // Copy row data
        currentBoard[i][WIDTH] = '\0'; // Ensure null-termination for safety
    }
}

// Display Mario's remaining lives on the board
void board::print_legend(const int num) const {
    gotoxy(LIFE_X, LIFE_Y); // Move cursor to lives display position
    std::cout << "life:" << std::string(LIVES, ' '); // Clear previous life display
    gotoxy(LIFE_X, LIFE_Y); // Reset cursor position for life display
    std::cout << "life:" << std::string(num, mario); // Display remaining lives
}

// Allow the player to choose a symbol for Mario
void board::chooseMarioSymbol() {
    std::cout << "Choose your Mario symbol:\n";
    std::cout << "1. @\n";
    std::cout << "2. *\n";
    std::cout << "3. #\n";
    std::cout << "Enter your choice: ";
    char choice = _getch(); // Get user input without echoing it

    switch (choice) {
    case '1':
        marioSymbol = '@'; // Set symbol to '@'
        break;
    case '2':
        marioSymbol = '*'; // Set symbol to '*'
        break;
    case '3':
        marioSymbol = '#'; // Set symbol to '#'
        break;
    default:
        std::cout << "Invalid choice. Default symbol (@) selected.\n"; // Handle invalid input
        Sleep(100); // Brief pause
        marioSymbol = '@'; // Default to '@'
        break;
    }
    setMarioSymbol(marioSymbol); // Apply the selected Mario symbol
}

void board::randomlyChangeEndPoint()
{
    Point newEndPos{ 0, 0 };
    do {
        newEndPos = { rand() % WIDTH, rand() % WIDTH, 'X' };
    } while (getChar(newEndPos) != ' ');
    if (!isSilent) {
        for (int i = 0; i < 5; i++) {
            // animate end point change (TODO: move to helper function)
            endPos.draw();
            newEndPos.erase();
            Sleep(10);
            endPos.erase();
            newEndPos.draw();
            Sleep(10);
        }
    }
    endPos = newEndPos;
}

// Load the board layout from a file and validate its contents
bool board::load(const std::string& filename) {
    std::ifstream screen_file(filename); // Open the file
    if (!screen_file.is_open()) { // Check if file opening failed
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    // Initialize row and column variables
    int curr_row = 0;
    int curr_col = 0;

    // Flags to ensure the presence of Mario and Princess symbols
    bool doesMarioExsist = false;
    bool doesPrincessExsist = false;

    int marioCount = 0; // Ensure only one Mario is present

    char c;
    // Read characters from the file and build the board
    while (!screen_file.get(c).eof() && curr_row < LENGTH) {
        if (c == '\n') {
            // Fill remaining columns with spaces if the row is too short
            if (curr_col < WIDTH) {
                std::fill(create_board[curr_row] + curr_col, create_board[curr_row] + WIDTH, ' ');
            }
            ++curr_row; // Move to the next row
            curr_col = 0; // Reset column index
        }
        else if (curr_col < WIDTH) { // Handle valid characters within board dimensions
            if (c == marioSymbol) { // Handle Mario's position
                marioCount++;
                doesMarioExsist = true;
                mario_x = curr_col; // Save Mario's position
                mario_y = curr_row;
                create_board[curr_row][curr_col++] = ' '; // Replace Mario symbol with space
            }
            else if (c == princess) { // Handle Princess's position
                doesPrincessExsist = true;
                princess_x = curr_col;
                princess_y = curr_row;
                create_board[curr_row][curr_col++] = c;
            }
            else if (c == 'L') { // Handle life symbol position
                LIFE_X = curr_col;
                LIFE_Y = score_Y = curr_row;
                score_X = curr_col + 9;
                create_board[curr_row][curr_col++] = c;
            }
            else { // Add other characters to the board
                create_board[curr_row][curr_col++] = c;
            }
        }
    }

    // Fill remaining spaces for the last row if shorter than the width
    if (curr_col < WIDTH && curr_row < LENGTH) {
        std::fill(create_board[curr_row] + curr_col, create_board[curr_row] + WIDTH, ' ');
    }

    // Validate presence of Mario and Princess
    if (!doesMarioExsist || !doesPrincessExsist) {
        std::cerr << "Error: Mario or Princess is missing." << std::endl;
        return false;
    }

    // Ensure only one Mario is present
    if (marioCount > 1) {
        std::cerr << "Error: More than one Mario on the screen." << std::endl;
        return false;
    }

    int last_row = (curr_row < LENGTH ? curr_row : LENGTH - 1);

    // Add a closing frame around the board
    std::fill(create_board[0], create_board[0] + WIDTH, wall); // Top border
    std::fill(create_board[last_row], create_board[last_row] + WIDTH, wall); // Bottom border

    for (int row = 1; row < last_row; ++row) { // Side borders
        create_board[row][0] = wall;
        create_board[row][WIDTH - 1] = wall;
    }

    screen_file.close(); // Close the file
    return true; // Return success
}

void board::initializeTimer(bool fastMode) {

    if (start_time.time_since_epoch().count() == 0) {
        start_time = std::chrono::steady_clock::now();
        }
    is_fast_mode = fastMode;
    }


void board::updateAndPrintTimer() const {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();

    int minutes = elapsed / 60;
    int seconds = elapsed % 60;

    gotoxy(LIFE_X + 35, LIFE_Y);
    std::cout << "Time: " << std::setw(2) << std::setfill('0') << minutes
        << ":" << std::setw(2) << std::setfill('0') << seconds;
}

int board::getElapsedTime()
{
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count();
}
 