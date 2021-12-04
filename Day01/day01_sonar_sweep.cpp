#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

namespace
{
	std::vector<int> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			inputvec.push_back(std::stoi(line));
		}
		ifs.close();
	}

	int part1()
	{
		int increased = 0;

		if (!inputvec.empty()) {
			for (auto iter = inputvec.cbegin(); iter != inputvec.cend() - 1; ++iter) {
				if (*(iter + 1) > *iter) {
					++increased;
				}
			}
		}

		return increased;
	}

	int part2()
	{
		int increased = 0;
		std::vector<int> sumvec;

		if (inputvec.size() > 2) {
			for (auto iter = inputvec.cbegin(); iter != inputvec.cend() - 2; ++iter) {
				sumvec.push_back(*iter + *(iter + 1) + *(iter + 2));
			}
		}

		if (!sumvec.empty()) {
			for (auto iter = sumvec.cbegin(); iter != sumvec.cend() - 1; ++iter) {
				if (*(iter + 1) > *iter) {
					++increased;
				}
			}
		}

		return increased;
	}
}

void day01()
{
	// test input
	input("Day01/day01_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day01 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 7);

	auto p2_test_result = part2();
	std::cout << "[Day01 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 5);

	// puzzle input
	input("Day01/day01_input");

	auto p1_result = part1();
	std::cout << "[Day01 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 1167);

	auto p2_result = part2();
	std::cout << "[Day01 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1130);

	std::cout << "--------------------------------------------------\n";
}
