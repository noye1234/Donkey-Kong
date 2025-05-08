#include "utils.h"
#include "barrels.h"
#include "point.h" 
#include "game.h"

// The function moves the console cursor to the specified (x, y) coordinates in the console window.
void gotoxy(int x, int y) {
    std::cout.flush();
    COORD coord; // Create a COORD structure  
    coord.X = x; // Set the X coordinate 
    coord.Y = y; // Set the Y coordinate 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 

}

// The function is used to change the visibility state of the cursor in the Windows console.
void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

//Reads the character at a specific position (x, y) in the console. (from chat GPT)
char readCharFromConsole(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CHAR_INFO buffer[1];
    COORD bufferSize = { 1, 1 };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT readRegion = { x, y, x, y };

    if (ReadConsoleOutput(hConsole, buffer, bufferSize, bufferCoord, &readRegion)) {
        return buffer[0].Char.AsciiChar;
    }
    else {
        DWORD error = GetLastError();
        std::cerr << "Error reading console output: " << error << std::endl;
        return ' '; // תו ריק במקרה של שגיאה
    }
}

//Determines if a character indicates that Mario is falling.
bool is_falling(const char ch) {
   // if ((ch != middleFloor) && (ch != leftFloor) && (ch != rightFloor)) {
    if(ch ==space){
        return true;
    }
    else {
        return false;
    }
}
//Checks if there is a ladder at a given position.
bool is_there_ladder(int x, int y) {
    char ch;
    ch = Game::get_char(x, y);

    if (ch == ladder_symbol)
        return true;
    else
        return false;
}

bool is_floor(int x, int y) {
    char ch = Game::get_char(x, y);
    if (ch == middleFloor || ch == leftFloor || ch == rightFloor)
        return true;
    return false;
}


//Turns off the game by setting the game state to `false`.
void off() {
    Game::change_on(false);
}

//new2
bool get_silence() {
    return Game::get_silence();
}
long get_random_seed() { 
    return Game::get_random_seed(); 
}


bool is_save() {
    return Game::get_is_save();
}


int biased_random() {
    int probability = rand() % 100; 
    return (probability < 95) ? 1 : 0; 
}


bool is_it_special_char(char ch) {
    return(ch == leftFloor || ch == rightFloor || ch == middleFloor || ch == wall);
} 


// Utility function to clear the screen
void clearScreen() {
    system("cls"); // Clears the console screen
}

// Utility function to wait for valid key input
char waitForValidInput(const std::vector<char>& validKeys) {
    char input;
    do {
        input = _getch(); // Waits for key press
    } while (std::find(validKeys.begin(), validKeys.end(), input) == validKeys.end()); // Ensures the input is valid
    return input; // Returns the valid input
}