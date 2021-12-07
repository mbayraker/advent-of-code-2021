#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <numeric>

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
			std::stringstream ss{ line };
			for (std::string valstr{}; std::getline(ss, valstr, ',');) {
				inputvec.push_back(std::stoi(valstr));
			}
		}
		ifs.close();
	}

	int part1()
	{
		auto testvec{ inputvec };

		std::sort(testvec.begin(), testvec.end());
		auto median = testvec[testvec.size() / 2];

		int fuel = 0;
		for (const auto& i : testvec) {
			fuel += std::abs(i - median);
		}

		return fuel;
	}

	int part2()
	{
		int meanfloor = std::reduce(inputvec.begin(), inputvec.end(), 0) / inputvec.size();
		std::pair<int, int> mean{ meanfloor, (meanfloor + 1) };
		std::pair<int, int> fuel;

		for (const auto& i : inputvec) {
			fuel.first += (std::abs(i - mean.first) * (std::abs(i - mean.first) + 1)) / 2;
			fuel.second += (std::abs(i - mean.second) * (std::abs(i - mean.second) + 1)) / 2;
		}

		return std::min(fuel.first, fuel.second);
	}
}

void day07()
{
	// test input
	input("Day07/day07_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day07 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 37);

	auto p2_test_result = part2();
	std::cout << "[Day07 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 168);

	// puzzle input
	input("Day07/day07_input");

	auto p1_result = part1();
	std::cout << "[Day07 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 352331);

	auto p2_result = part2();
	std::cout << "[Day07 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 99266250);

	std::cout << "--------------------------------------------------\n";
}
