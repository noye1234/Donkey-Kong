#include "library.h"
#include "mario.h"
#include "barrels.h"
#include "ghosts.h"
#include "betterGhosts.h"
//NEW:
#include "Steps.h"
#include "Results.h"

#include "game.h"




// Game class definition
class GameSave : public Game {
    // Member variables
    std::vector<board> boards;                // Stores all boards
    std::vector<std::string> screen_files;   // Files corresponding to each screen
    std::vector<std::string> screen_name;    // Names of the screens
    char selected_screen;                    // Current screen selected




    int max_level = 3;       // Maximum levels in the game
    int curr_level = 1;      // Current level
    // static bool on;          // Tracks if the game is running
    board gameBoard;         // Current game board



    std::vector<std::string> fileNames;
    bool esc_pressed = false;
    bool unmatching_result_found = false;
    std::string filename_prefix;
    std::string stepsFilename;
    std::string resultsFilename;
   // long random_seed;

    Steps steps;
    Results results;

    size_t iteration = 0; // we need iteration to be outside the loop
    std::vector<std::string> screens;
public:

    void create_donkey_kong();
   // void make_hazard(std::vector<std::unique_ptr<hazard>>& hazard_array, Mario* mario);

};
