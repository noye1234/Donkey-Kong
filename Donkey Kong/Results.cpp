

#include "Results.h"

Results Results::loadResults(const std::string& filename) {
	std::ifstream results_file(filename);
	Results results;
	size_t size;
	results_file >> size;
	while (!results_file.eof() && size-- != 0) {
		size_t iteration;
		int result;
		results_file >> iteration >> result;
		results.addResult(iteration, static_cast<ResultValue>(result));
	}
	return results;
}

void Results::saveResults(const std::string& filename)  {
	std::ofstream results_file(filename);
	results_file << results.size();
	for (const auto& result : results) {
		results_file << '\n' << result.first << ' ' << result.second;
	}
	results_file.close();
	results.clear();

}

bool Results::isFinishedBy(size_t iteration) const
{
	if (results.empty()) {
		return false;  // if theres no result and we finished
	}

	// cheack if the last result is (Finished)
	auto lastResult = results.back();
	if (lastResult.first <= iteration && lastResult.second == Results::finished) {
		return true;
	}

	return false;

}

size_t Results::getNextBombIteration() const {
	if (!results.empty() && results.front().second == hitBomb) {
		return results.front().first;
	}
	else return std::numeric_limits<size_t>::max(); // a big number we will never reach
}
