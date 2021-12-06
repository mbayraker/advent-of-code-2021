#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <array>

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

	unsigned long long calculate_fish_count(int daycnt, std::vector<int>& invec)
	{
		unsigned long long totalcnt = 0;
		std::array<unsigned long long, 9> lifearr{};

		for (const auto& fish : invec) {
			lifearr[fish]++;
		}

		for (int d = 0; d < daycnt; ++d) {
			lifearr[7] += lifearr[0];
			std::rotate(lifearr.begin(), lifearr.begin() + 1, lifearr.end());
		}

		for (const auto& fcnt : lifearr) {
			totalcnt += fcnt;
		}

		return totalcnt;
	}

	unsigned long long part1()
	{
		return calculate_fish_count(80, inputvec);
	}

	unsigned long long part2()
	{
		return calculate_fish_count(256, inputvec);
	}
}

void day06()
{
	// test input
	input("Day06/day06_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day06 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 5934);

	auto p2_test_result = part2();
	std::cout << "[Day06 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 26984457539);

	// puzzle input
	input("Day06/day06_input");

	auto p1_result = part1();
	std::cout << "[Day06 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 363101);

	auto p2_result = part2();
	std::cout << "[Day06 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1644286074024);

	std::cout << "--------------------------------------------------\n";
}
