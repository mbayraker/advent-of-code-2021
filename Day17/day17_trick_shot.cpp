#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>

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

		ifs.ignore(std::numeric_limits<std::streamsize>::max(), '=');
		for (std::string line{}; std::getline(ifs, line);) {
			std::stringstream ss{ line };
			int val{};
			if (ss >> val) {
				inputvec.push_back(val); // xmin
			}
			ss.ignore(2);
			if (ss >> val) {
				inputvec.push_back(val); // xmax
			}
			ss.ignore(std::numeric_limits<std::streamsize>::max(), '=');
			if (ss >> val) {
				inputvec.push_back(val); // ymin
			}
			ss.ignore(2);
			if (ss >> val) {
				inputvec.push_back(val); // ymax
			}
		}
		ifs.close();
	}

	auto trickShot()
	{
		const int xmin = inputvec[0], xmax = inputvec[1],
			      ymin = inputvec[2], ymax = inputvec[3];

		int yhighest = std::numeric_limits<int>::min(), hit = 0;

		// asume x positive and y negative
		for (int DX = 0; DX <= xmax; ++DX) {
			for (int DY = ymin; DY <= std::abs(ymin); ++DY) {
				int x = 0, y = 0, dx = DX, dy = DY,
					ytop = std::numeric_limits<int>::min();
				bool shot = false;
				
				while (true) {
					x += dx;
					y += dy;
					ytop = std::max(ytop, y);

					// drag
					if (dx > 0) {
						dx -= 1;
					}
					else if (dx < 0) {
						dx += 1;
					}

					// gravity
					dy -= 1;

					if (x >= xmin && x <= xmax && y >= ymin && y <= ymax) {
						shot = true;
						if (ytop > yhighest) {
							yhighest = ytop;
						}
					}

					// overshoot
					if (y < ymin) {
						break;
					}
				}

				// count only one per initial velocity
				if (shot) {
					++hit;
				}
			}
		}

		return std::make_pair(yhighest, hit);
	}

	int part1()
	{
		auto result = trickShot();
		return result.first;
	}

	int part2()
	{
		auto result = trickShot();
		return result.second;
	}
}

void day17()
{
	// test input
	input("Day17/day17_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day17 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 45);

	auto p2_test_result = part2();
	std::cout << "[Day17 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 112);

	// puzzle input
	input("Day17/day17_input");

	auto p1_result = part1();
	std::cout << "[Day17 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 14535);

	auto p2_result = part2();
	std::cout << "[Day17 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 2270);

	std::cout << "--------------------------------------------------\n";
}
