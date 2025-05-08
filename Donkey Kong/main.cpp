
#include "game.h"
#include "GameLoad.h"
#include "GameSave.h"
#include "board.h"


// TODO: Better put this is a proper class
void getAllBoardFileNames(std::vector<std::string>& vec_to_fill) {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 5) == "board" && filename.extension() == ".screen") {
			std::cout << " ^ added!!\n";
			vec_to_fill.push_back(filenameStr);
		}
	}
}

// TODO: Better put this is a proper class
void reportResultError(const std::string& message, const std::string& filename, size_t iteration) {
	system("cls");
	std::cout << "Screen " << filename << " - " << message << '\n';
	std::cout << "Iteration: " << iteration << '\n';
	std::cout << "Press any key to continue to next screens (if any)" << std::endl;
	_getch();
}

int main(int argc, char** argv) {

	ShowConsoleCursor(false);
	bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
	bool issave = argc > 1 && std::string(argv[1]) == "-save";
	bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";

	Game* game;
	if (isLoad) {
		game = new GameLoad;
		if (isSilent)
			game->set_silence(true);
	}
	else{
		game = new GameSave;
		if (issave)
			game->set_save(true);
	}
	game->run();
	delete game;

    return 0; 
}


