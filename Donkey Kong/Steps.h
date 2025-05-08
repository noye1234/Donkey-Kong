#pragma once

#include <list>
#include <string>
#include <iostream>

class Steps {
	long randomSeed = 0;
	char selected_screen ='1';
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step
public:
	static Steps loadSteps(const std::string& filename);
	void saveSteps(const std::string& filename);
	long getRandomSeed() const {
		return randomSeed;
	}

	char get_selected_screen() const {
		return selected_screen;
	}
	void clear()  {
		steps.clear();
	}


	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void addStep(size_t iteration, char step) {
		steps.push_back({ iteration, step });
	}
	bool isNextStepOnIteration(size_t iteration) const {
		return ((!steps.empty()) && (steps.front().first == iteration));
	}
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}

	bool is_empty() { return steps.empty(); }
};
