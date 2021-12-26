#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>

namespace
{
	std::vector<std::string> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			inputvec.push_back(line);
		}
		ifs.close();
	}

	bool step(std::vector<std::string>& vec, int rnum, int cnum)
	{
		bool moved = false;

		// east-facing
		auto tmpvec{ vec };
		for (int i = 0; i < rnum; ++i) {
			for (int j = 0; j < cnum; ++j) {
				int next = (j + 1) % cnum;
				if (vec[i][j] == '>' && vec[i][next] == '.') {
					std::swap(tmpvec[i][j], tmpvec[i][next]);
					moved = true;
				}
			}
		}
		vec = tmpvec;

		// south-facing
		for (int i = 0; i < rnum; ++i) {
			for (int j = 0; j < cnum; ++j) {
				int next = (i + 1) % rnum;
				if (vec[i][j] == 'v' && vec[next][j] == '.') {
					std::swap(tmpvec[i][j], tmpvec[next][j]);
					moved = true;
				}
			}
		}
		vec = std::move(tmpvec);

		return moved;
	}

	int part1()
	{
		auto cucumbers{ inputvec };
		auto R = cucumbers.size();
		auto C = cucumbers[0].size();

		int stepcnt = 0;
		while (step(cucumbers, R, C)) {
			++stepcnt;
		}

		return stepcnt + 1;
	}
}

void day25()
{
	// test input
	input("Day25/day25_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day25 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 58);

	// puzzle input
	input("Day25/day25_input");

	auto p1_result = part1();
	std::cout << "[Day25 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 380);

	std::cout << "--------------------------------------------------\n";
}
