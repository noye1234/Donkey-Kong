#pragma once
#include <list>
#include <limits>
#include <fstream>

class Results
{
public:
	//amir aprooved regular enum when inside of class 
	enum ResultValue { hitBomb, finished, noResult };
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	static Results loadResults(const std::string& filename);
	void saveResults(const std::string& filename) ;
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const;

	size_t getNextBombIteration() const;

	bool is_empty() { return results.empty(); }

};