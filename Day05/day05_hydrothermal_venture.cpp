#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <map>

namespace
{
	struct Line {
		Line(int x1, int y1, int x2, int y2) : pos{ x1, y1, x2, y2 } {}

		std::tuple<int, int, int, int> pos;
	};

	std::vector<Line> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			const std::string delim{ " -> " };
			if (auto delpos = line.find(delim); delpos != std::string::npos) {
				std::vector<int> nums;

				std::stringstream ss{ line.substr(0, delpos) };
				for (std::string valstr{}; std::getline(ss, valstr, ',');) {
					nums.push_back(std::stoi(valstr));
				}

				ss = std::stringstream{ line.substr(delpos + delim.length()) };
				for (std::string valstr{}; std::getline(ss, valstr, ',');) {
					nums.push_back(std::stoi(valstr));
				}

				inputvec.emplace_back(nums[0], nums[1], nums[2], nums[3]);
			}
		}
		ifs.close();
	}

	int part1()
	{
		int result = 0;
		std::map<std::pair<int, int>, int> diagmap;

		for (const auto& vent : inputvec) {
			const auto &[x1, y1, x2, y2] = vent.pos;

			if (x1 == x2) {
				// vertical
				for (int i = std::min(y1, y2); i <= std::max(y1, y2); ++i) {
					diagmap[std::make_pair(i, x1)]++;
				}
			}
			else if (y1 == y2) {
				// horizontal
				for (int i = std::min(x1, x2); i <= std::max(x1, x2); ++i) {
					diagmap[std::make_pair(y1, i)]++;
				}
			}
		}

		// count the points where at least two lines overlap
		auto foverlap = [](const auto& p) {
			return p.second >= 2;
		};
		result = std::count_if(diagmap.cbegin(), diagmap.cend(), foverlap);

		return result;
	}

	int part2()
	{
		int result = 0;
		std::map<std::pair<int, int>, int> diagmap;

		for (const auto& vent : inputvec) {
			const auto &[x1, y1, x2, y2] = vent.pos;

			if (x1 == x2) {
				// vertical
				for (int i = std::min(y1, y2); i <= std::max(y1, y2); ++i) {
					diagmap[std::make_pair(i, x1)]++;
				}
			}
			else if (y1 == y2) {
				// horizontal
				for (int i = std::min(x1, x2); i <= std::max(x1, x2); ++i) {
					diagmap[std::make_pair(y1, i)]++;
				}
			}
			else if (std::abs(x1 - x2) == std::abs(y1 - y2)) {
				// diagonal
				int upx = (x1 < x2) ? 1 : -1;
				int upy = (y1 < y2) ? 1 : -1;

				for (auto dx{ x1 }, dy{ y1 }; dx != (x2 + upx); dx += upx, dy += upy) {
					diagmap[std::make_pair(dy, dx)]++;
				}
			}
		}

		// count the points where at least two lines overlap
		auto foverlap = [](const auto& p) {
			return p.second >= 2;
		};
		result = std::count_if(diagmap.cbegin(), diagmap.cend(), foverlap);

		return result;
	}
}

void day05()
{
	// test input
	input("Day05/day05_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day05 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 5);

	auto p2_test_result = part2();
	std::cout << "[Day05 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 12);

	// puzzle input
	input("Day05/day05_input");

	auto p1_result = part1();
	std::cout << "[Day05 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 5442);

	auto p2_result = part2();
	std::cout << "[Day05 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 19571);

	std::cout << "--------------------------------------------------\n";
}
