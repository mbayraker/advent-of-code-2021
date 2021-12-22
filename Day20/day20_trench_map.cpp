#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <set>

namespace
{
	using Grid = std::set<std::pair<int, int>>;

	std::string enhancealgo;
	std::vector<std::string> inputvec;

	void input(const char* fpath)
	{
		enhancealgo.clear();
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		std::getline(ifs, enhancealgo);

		for (std::string line{}; std::getline(ifs, line);) {
			if (line.empty()) {
				continue;
			}
			inputvec.push_back(line);
		}
		ifs.close();
	}

	Grid prepGrid()
	{
		Grid grid;

		for (size_t r = 0; r < inputvec.size(); ++r) {
			for (size_t c = 0; c < inputvec[0].size(); ++c) {
				if (inputvec[r][c] == '#') {
					grid.insert({ r, c });
				}
			}
		}

		return grid;
	}

	auto getBoundaries(const Grid& g)
	{
		auto [mincIter, maxcIter] = std::minmax_element(g.cbegin(), g.cend(),
			[](const auto& a, const auto& b) {
				return a.second < b.second;
			});

		int minCol = (*mincIter).second;
		int maxCol = (*maxcIter).second;

		int minRow = (*g.begin()).first;
		int maxRow = (*g.rbegin()).first;

		return std::make_tuple(minRow, maxRow, minCol, maxCol);
	}

	bool getPixel(Grid& g, int row, int col, bool on)
	{
		bool retval = on ? true : false;

		if (g.count({ row, col })) {
			return retval;
		}
		else {
			return !retval;
		}
	}

	int enhance(Grid& g, size_t times)
	{
		bool alternate = false;
		if (enhancealgo[0] == '#' && enhancealgo[511] == '.') {
			alternate = true;
		}

		for (size_t run = 0; run < times; ++run) {
			Grid dest;
			auto [minR, maxR, minC, maxC] = getBoundaries(g);

			// if not alternating, always keep track of light pixels
			// otherwise, alternate tracking between iterations (start with light)
			bool trackOn = alternate ? (run % 2 == 0) : true;
			
			for (int row = minR - 1; row <= maxR + 1; ++row) {
				for (int col = minC - 1; col <= maxC + 1; ++col) {
					std::bitset<9> bin;

					bin.set(8, getPixel(g, row - 1, col - 1, trackOn));
					bin.set(7, getPixel(g, row - 1, col, trackOn));
					bin.set(6, getPixel(g, row - 1, col + 1, trackOn));

					bin.set(5, getPixel(g, row, col - 1, trackOn));
					bin.set(4, getPixel(g, row, col, trackOn));
					bin.set(3, getPixel(g, row, col + 1, trackOn));

					bin.set(2, getPixel(g, row + 1, col - 1, trackOn));
					bin.set(1, getPixel(g, row + 1, col, trackOn));
					bin.set(0, getPixel(g, row + 1, col + 1, trackOn));

					auto algochar = enhancealgo[bin.to_ulong()];
					if (alternate) {
						bool destTrack = !trackOn;

						// insert based on the tracking status
						if (destTrack && (algochar == '#')) {
							dest.insert({ row, col });
						}
						else if (!destTrack && (algochar == '.')) {
							dest.insert({ row, col });
						}
					}
					else {
						// insert light pixels
						if (algochar == '#') {
							dest.insert({ row, col });
						}
					}
				}
			}

			// move for next iteration
			g = std::move(dest);
		}

		return g.size();
	}

	int part1()
	{
		Grid grid{ prepGrid() };
		return enhance(grid, 2);
	}

	int part2()
	{
		Grid grid{ prepGrid() };
		return enhance(grid, 50);
	}
}

void day20()
{
	// test input
	input("Day20/day20_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day20 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 35);

	auto p2_test_result = part2();
	std::cout << "[Day20 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 3351);

	// puzzle input
	input("Day20/day20_input");

	auto p1_result = part1();
	std::cout << "[Day20 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 5097);

	auto p2_result = part2();
	std::cout << "[Day20 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 17987);

	std::cout << "--------------------------------------------------\n";
}
