#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <array>
#include <algorithm>

namespace
{
	struct BingoBoard {
		constexpr static int GRID_SIZE = 5;

		std::array<std::array<int, GRID_SIZE>, GRID_SIZE> nums{};
		bool bingo{};

		bool isBingo() const
		{
			for (int i = 0; i < GRID_SIZE; ++i) {
				// check rows
				auto rowcheck = [](const auto& x) {
					return x == -1;
				};

				if (std::all_of(nums[i].cbegin(), nums[i].cend(), rowcheck)) {
					return true;
				}

				// check cols
				auto colcheck = [i](const auto& v) {
					return v[i] == -1;
				};

				if (std::all_of(nums.cbegin(), nums.cend(), colcheck)) {
					return true;
				}
			}

			return false;
		}
	};

	std::vector<int> drawvec;
	std::vector<BingoBoard> boardvec;

	void input(const char* fpath)
	{
		drawvec.clear();
		boardvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		// parse draw numbers
		if (std::string drawstr{}; std::getline(ifs, drawstr)) {
			std::stringstream ss{ drawstr };
			for (std::string numstr{}; std::getline(ss, numstr, ',');) {
				drawvec.push_back(std::stoi(numstr));
			}
		}

		// parse boards
		int boardnum = 0, row = 0;
		for (std::string rowstr{}; std::getline(ifs, rowstr);) {
			if (rowstr.length()) {
				if (boardnum == 0) {
					boardvec.push_back( BingoBoard{} );
					boardnum++;
				}

				if (row == BingoBoard::GRID_SIZE) {
					boardvec.push_back( BingoBoard{} );
					boardnum++;
					row = 0;
				}

				std::stringstream ss{ rowstr };
				for (int col = 0, val = 0; ss >> val; ++col) {
					boardvec[boardnum - 1].nums[row][col] = val;
				}
				row++;
			}
		}
		ifs.close();
	}

	int part1()
	{
		std::vector<BingoBoard> testboardvec{ boardvec };

		for (const auto& draw : drawvec) {
			for (auto& board : testboardvec) {
				// mark the number on the board
				for (int row = 0; row < BingoBoard::GRID_SIZE; ++row) {
					std::replace(board.nums[row].begin(), board.nums[row].end(), draw, -1);
				}

				if (board.isBingo()) {
					// calculate the sum of unmarked numbers
					int unmarkedsum = 0;

					for (int row = 0; row < BingoBoard::GRID_SIZE; ++row) {
						auto fsum = [&unmarkedsum](const auto& x) {
							if (x != -1) { unmarkedsum += x; }
						};

						std::for_each(board.nums[row].cbegin(), board.nums[row].cend(), fsum);
					}

					int result = unmarkedsum * draw;
					return result;
				}
			}
		}

		return 0;
	}

	int part2()
	{
		std::vector<BingoBoard> testboardvec{ boardvec };
		size_t bingocnt = 0;

		for (const auto& draw : drawvec) {
			for (auto& board : testboardvec) {
				// mark the number on the board
				for (int row = 0; row < BingoBoard::GRID_SIZE; ++row) {
					std::replace(board.nums[row].begin(), board.nums[row].end(), draw, -1);
				}

				if (board.isBingo()) {
					if (!board.bingo) {
						bingocnt++;
						board.bingo = true;
					}

					if (bingocnt == testboardvec.size()) {
						// calculate the sum of unmarked numbers
						int unmarkedsum = 0;

						for (int row = 0; row < BingoBoard::GRID_SIZE; ++row) {
							auto fsum = [&unmarkedsum](const auto& x) {
								if (x != -1) { unmarkedsum += x; }
							};

							std::for_each(board.nums[row].cbegin(), board.nums[row].cend(), fsum);
						}

						int result = unmarkedsum * draw;
						return result;
					}
				}
			}
		}

		return 0;
	}
}

void day04()
{
	// test input
	input("Day04/day04_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day04 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 4512);

	auto p2_test_result = part2();
	std::cout << "[Day04 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 1924);

	// puzzle input
	input("Day04/day04_input");

	auto p1_result = part1();
	std::cout << "[Day04 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 44736);

	auto p2_result = part2();
	std::cout << "[Day04 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1827);

	std::cout << "--------------------------------------------------\n";
}
