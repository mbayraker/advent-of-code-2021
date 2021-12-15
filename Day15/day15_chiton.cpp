#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <queue>

namespace
{
	std::vector<std::vector<int>> weightvec;

	void input(const char* fpath)
	{
		weightvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		int row = 0;
		for (std::string line{}; std::getline(ifs, line);) {
			weightvec.emplace_back();
			for (auto c : line) {
				weightvec[row].push_back(c - '0');
			}
			++row;
		}
		ifs.close();
	}

	int lowestRisk(int srow, int scol, int tile)
	{
		const int wrnum = weightvec.size();    // num of rows for top-left tile
		const int wcnum = weightvec[0].size(); // num of cols for top-left tile
		const int RNUM = wrnum * tile;         // num of rows for grid
		const int CNUM = wcnum * tile;         // num of cols for grid
		constexpr std::pair<int, int> dirs[4] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

		// dijkstra
		using D = std::tuple<int, int, int>; // distance, r, c
		std::priority_queue<D, std::vector<D>, std::greater<D>> pque; // min heap
		std::vector<std::vector<int>> dists(RNUM,
			std::vector<int>(CNUM, std::numeric_limits<int>::max()));

		pque.emplace(0, srow, scol);

		while (!pque.empty()) {
			auto [d, r, c] = pque.top();
			pque.pop();

			if (r < 0 || c < 0 || r >= RNUM || c >= CNUM) {
				// out of bounds
				continue;
			}

			// calculate the weight using the inner tile weight
			auto w = weightvec[r % wrnum][c % wcnum] + (r / wrnum) + (c / wcnum);
			w = (w > 9) ? (w - 9) : w;

			if (auto newd = d + w; newd < dists[r][c]) {
				// found shorter path
				dists[r][c] = newd;
			}
			else {
				continue;
			}

			for (int i = 0; i < 4; ++i) {
				pque.emplace(dists[r][c], (r + dirs[i].first), (c + dirs[i].second));
			}
		}

		// remove the starting weight
		int ans = dists[RNUM - 1][CNUM - 1] - weightvec[srow][scol];
		return ans;
	}

	int part1()
	{
		return lowestRisk(0, 0, 1);
	}

	int part2()
	{
		return lowestRisk(0, 0, 5);
	}
}

void day15()
{
	// test input
	input("Day15/day15_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day15 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 40);

	auto p2_test_result = part2();
	std::cout << "[Day15 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 315);

	// puzzle input
	input("Day15/day15_input");

	auto p1_result = part1();
	std::cout << "[Day15 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 553);

	auto p2_result = part2();
	std::cout << "[Day15 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 2858);

	std::cout << "--------------------------------------------------\n";
}
