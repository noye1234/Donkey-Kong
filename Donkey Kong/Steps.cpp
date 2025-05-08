#include <fstream>

#include "Steps.h"
#include "game.h"

Steps Steps::loadSteps(const std::string& filename) {
	Steps steps;
	std::ifstream steps_file(filename);
	steps_file >> steps.randomSeed;

	//steps_file >> steps.selected_screen;

	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();
	return steps;
}

void Steps::saveSteps(const std::string& filename)  {
	std::ofstream steps_file(filename);

	steps_file << randomSeed << '\n'  << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
	steps.clear();
}
