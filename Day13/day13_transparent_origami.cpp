#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>

namespace
{
	std::vector<std::pair<int, int>> inputvec;
	std::vector<std::pair<char, int>> foldvec;

	struct Grid {
		Grid(size_t rowsize, size_t colsize) : g{ colsize, std::vector<char>(rowsize, '.') } {}

		void print() const {
			for (size_t i = 0; i < g.size(); ++i) {
				for (size_t j = 0; j < g[0].size(); ++j) {
					std::cout << g[i][j];
				}
				std::cout << "\n";
			}
		}

		size_t countDots() const {
			size_t dotCnt = 0;
			for (const auto& cvec : g) {
				dotCnt += std::count(cvec.cbegin(), cvec.cend(), '#');
			}
			return dotCnt;
		}

		std::vector<std::vector<char>> g;
	};

	void input(const char* fpath)
	{
		inputvec.clear();
		foldvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			if (auto delpos = line.find(','); delpos != std::string::npos) {
				inputvec.emplace_back( std::stoi(line.substr(0, delpos)),
                                       std::stoi(line.substr(delpos + 1)) );
			}
			else if (delpos = line.find('='); delpos != std::string::npos) {
				foldvec.emplace_back( line.substr(delpos - 1, 1)[0],
                                      std::stoi(line.substr(delpos + 1)) );
			}
		}
		ifs.close();
	}

	Grid makeGrid()
	{
		auto fx = [](const auto& a, const auto& b) {
			return a.first < b.first;
		};

		auto fy = [](const auto& a, const auto& b) {
			return a.second < b.second;
		};

		size_t rowsize = std::max_element(inputvec.cbegin(), inputvec.cend(), fx)->first + 1;
		size_t colsize = std::max_element(inputvec.cbegin(), inputvec.cend(), fy)->second + 1;

		Grid grid{ rowsize, colsize };

		for (const auto& p : inputvec) {
			grid.g[p.second][p.first] = '#';
		}

		return grid;
	}

	void foldGrid(Grid& grid, char dir, int line)
	{
		size_t rsize = grid.g[0].size();
		size_t csize = grid.g.size();

		if (dir == 'x') {
			for (size_t i = 0; i < csize; ++i) {
				for (size_t j = line + 1; j < rsize; ++j) {
					if (grid.g[i][j] == '#') {
						grid.g[i][line - (j - line)] = grid.g[i][j];
					}
				}
			}
			
			// erase after folding
			for (auto& cvec : grid.g) {
				cvec.erase(cvec.begin() + line, cvec.end());
			}
		}
		else if (dir == 'y') {
			for (size_t i = line + 1; i < csize; ++i) {
				for (size_t j = 0; j < rsize; ++j) {
					if (grid.g[i][j] == '#') {
						grid.g[line - (i - line)][j] = grid.g[i][j];
					}
				}
			}

			// erase after folding
			grid.g.erase(grid.g.begin() + line, grid.g.end());
		}
	}

	size_t part1()
	{
		Grid grid = makeGrid();

		foldGrid(grid, foldvec[0].first, foldvec[0].second);
		return grid.countDots();
	}

	void part2()
	{
		Grid grid = makeGrid();

		for (const auto& fold : foldvec) {
			foldGrid(grid, fold.first, fold.second);
		}
		grid.print();
	}
}

void day13()
{
	// test input
	input("Day13/day13_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day13 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 17);

	std::cout << "[Day13 - P2] Test Answer   =\n";
	part2(); // Answer: 0

	// puzzle input
	input("Day13/day13_input");

	auto p1_result = part1();
	std::cout << "[Day13 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 682);

	std::cout << "[Day13 - P2] Puzzle Answer =\n";
	part2(); // Answer: FAGURZHE

	std::cout << "--------------------------------------------------\n";
}
