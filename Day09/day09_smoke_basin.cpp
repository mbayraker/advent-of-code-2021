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
	struct Cave {
		using Point = std::pair<int, char>;

		Cave(std::vector<std::string>& datvec, int n)
		{
			h.resize(n);

			int idx = 0;
			for (const auto& str : datvec) {
				for (const auto& c : str) {
					h[idx].emplace_back(idx, c);
					idx++;
				}
			}
		}

		void addEdgeData(int e1, int e2, char d1, char d2)
		{
			h[e1].emplace_back(e2, d2);
			h[e2].emplace_back(e1, d1);
		}

		std::vector<std::vector<Point>> h{};
	};

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

	Cave prepCave()
	{
		int hcount = 0;

		std::for_each(inputvec.cbegin(), inputvec.cend(),
			[&hcount](const auto& s) {
				hcount += s.length();
			});

		Cave cave(inputvec, hcount);

		for (size_t i = 0, node = 0; i < inputvec.size(); ++i) {
			for (size_t j = 0; j < inputvec[0].size(); ++j) {
				// horizontal edges
				if (j != (inputvec[0].size() - 1)) {
					cave.addEdgeData(node, node + 1,
						inputvec[i][j], inputvec[i][j + 1]);
				}

				// vertical edges
				if (i != (inputvec.size() - 1)) {
					cave.addEdgeData(node, node + inputvec[0].size(),
						inputvec[i][j], inputvec[i + 1][j]);
				}

				node++;
			}
		}

		return cave;
	}

	int part1()
	{
		int sum = 0;
		Cave cave{ prepCave() };

		// find the low points and add 1
		for (const auto& hvec : cave.h) {
			auto f = [](const auto& a, const auto& b) {
				return a.second < b.second;
			};
			auto min = *std::min_element(hvec.cbegin() + 1, hvec.cend(), f);

			if (hvec[0].second < min.second) {
				sum += hvec[0].second - '0' + 1;
			}
		}

		return sum;
	}

	int part2()
	{
		Cave cave{ prepCave() };
		std::vector<Cave::Point> lowpoints{};
		std::vector<int> basin{};

		// find the low points
		for (const auto& hvec : cave.h) {
			auto f = [](const auto& a, const auto& b) {
				return a.second < b.second;
			};
			auto min = *std::min_element(hvec.cbegin() + 1, hvec.cend(), f);

			if (hvec[0].second < min.second) {
				lowpoints.push_back(hvec[0]);
			}
		}

		// bfs on each low point
		for (const auto& lp : lowpoints) {
			std::vector<bool> visitvec(cave.h.size(), false);
			std::queue<Cave::Point> bfsque;
			
			visitvec[lp.first] = true;
			bfsque.push(lp);
			int basincnt = 1;

			while (!bfsque.empty()) {
				auto p = bfsque.front();
				bfsque.pop();

				for (size_t i = 1; i < cave.h[p.first].size(); ++i) {
					auto adj = cave.h[p.first][i];
					
					if ((!visitvec[adj.first]) && (adj.second != '9') && (p.second < adj.second)) {
						visitvec[adj.first] = true;
						bfsque.push(adj);
						basincnt++;
					}
				}
			}

			basin.push_back(basincnt);
		}

		std::partial_sort(basin.begin(), basin.begin() + 3, basin.end(), std::greater{});
		int result = basin[0] * basin[1] * basin[2];

		return result;
	}
}

void day09()
{
	// test input
	input("Day09/day09_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day09 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 15);

	auto p2_test_result = part2();
	std::cout << "[Day09 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 1134);

	// puzzle input
	input("Day09/day09_input");

	auto p1_result = part1();
	std::cout << "[Day09 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 456);

	auto p2_result = part2();
	std::cout << "[Day09 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1047744);

	std::cout << "--------------------------------------------------\n";
}
