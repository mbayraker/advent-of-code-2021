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
	constexpr static size_t GRID_SIZE = 10;

	using OctopusArr = std::array<std::array<int, GRID_SIZE>, GRID_SIZE>;

	OctopusArr inputarr;

	void input(const char* fpath)
	{
		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		int row = 0;
		for (std::string line{}; std::getline(ifs, line);) {
			int col = 0;
			for (const auto &c : line) {
				inputarr[row][col] = c - '0';
				col++;
			}
			row++;
		}
		ifs.close();
	}

	bool is_valid(std::pair<int, int> pos, std::pair<int, int> dir)
	{
		auto x = pos.first + dir.first;
		auto y = pos.second + dir.second;

		if (x >= GRID_SIZE || y >= GRID_SIZE || x < 0 || y < 0) {
			return false;
		}

		return true;
	}

	int count_flash(OctopusArr& arr, std::pair<int, int> pos)
	{
		int flash = 0;

		if (arr[pos.first][pos.second] > 9) {
			arr[pos.first][pos.second] = 0;
			flash++;
		}
		else {
			return 0;
		}

		const std::array<std::pair<int, int>, 8> dirs{{
			{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
		}};

		for (const auto& dir : dirs) {
			if (is_valid(pos, dir)) {
				auto adjx = pos.first + dir.first;
				auto adjy = pos.second + dir.second;

				if (arr[adjx][adjy] && ++arr[adjx][adjy] > 9) {
					flash += count_flash(arr, std::make_pair(adjx, adjy));
				}
			}
		}

		return flash;
	}

	int part1()
	{
		OctopusArr testarr{ inputarr };
		int flashcnt = 0;
		constexpr size_t STEP_SIZE = 100;

		for (size_t step = 0; step < STEP_SIZE; ++step) {
			// increment
			for (auto& a : testarr) {
				std::transform(a.cbegin(), a.cend(), a.begin(), [](auto& x) { return x + 1; });
			}

			// count flash
			for (size_t i = 0; i < GRID_SIZE; ++i) {
				for (size_t j = 0; j < GRID_SIZE; ++j) {
					flashcnt += count_flash(testarr, std::make_pair(i, j));
				}
			}
		}

		return flashcnt;
	}

	int part2()
	{
		OctopusArr testarr{ inputarr };
		int syncstep = 0;

		while (++syncstep) {
			// increment
			for (auto& a : testarr) {
				std::transform(a.cbegin(), a.cend(), a.begin(), [](auto& x) { return x + 1; });
			}

			// count flash
			int flashcnt = 0;
			for (size_t i = 0; i < GRID_SIZE; ++i) {
				for (size_t j = 0; j < GRID_SIZE; ++j) {
					flashcnt += count_flash(testarr, std::make_pair(i, j));
				}
			}

			// check for sync flash
			if (flashcnt == GRID_SIZE * GRID_SIZE) {
				return syncstep;
			}
		}

		return 0;
	}
}

void day11()
{
	// test input
	input("Day11/day11_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day11 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 1656);

	auto p2_test_result = part2();
	std::cout << "[Day11 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 195);

	// puzzle input
	input("Day11/day11_input");

	auto p1_result = part1();
	std::cout << "[Day11 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 1546);

	auto p2_result = part2();
	std::cout << "[Day11 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 471);

	std::cout << "--------------------------------------------------\n";
}
