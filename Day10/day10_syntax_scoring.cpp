#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <stack>
#include <map>

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

	const std::string openchunk{ "([{<" };
	const std::string closechunk{ ")]}>" };

	unsigned long long part1()
	{
		const std::map<char, int> scoremap {
			{ ')', 3 },
			{ ']', 57 },
			{ '}', 1197 },
			{ '>', 25137 }
		};

		unsigned long long result = 0;
		for (const auto& s : inputvec) {
			std::stack<char> syntaxstack{};

			for (const auto& c : s) {
				if (openchunk.find(c) != std::string::npos) {
					syntaxstack.push(c);
				}
				else if (auto pos = closechunk.find(c); pos != std::string::npos) {
					if (!syntaxstack.empty()) {
						auto top = syntaxstack.top();
						syntaxstack.pop();

						// find corrupted lines
						if (openchunk.find(top) != pos) {
							result += scoremap.at(c);
							break;
						}
					}
				}
			}
		}

		return result;
	}

	unsigned long long part2()
	{
		const std::map<char, int> openscoremap {
			{ '(', 1 },
			{ '[', 2 },
			{ '{', 3 },
			{ '<', 4 }
		};

		std::vector<unsigned long long> resultvec;
		for (const auto& s : inputvec) {
			std::stack<char> syntaxstack{};
			bool ignore = false;

			for (const auto& c : s) {
				if (openchunk.find(c) != std::string::npos) {
					syntaxstack.push(c);
				}
				else if (auto pos = closechunk.find(c); pos != std::string::npos) {
					if (!syntaxstack.empty()) {
						auto top = syntaxstack.top();
						syntaxstack.pop();

						// ignore corrupted lines
						if (openchunk.find(top) != pos) {
							ignore = true;
							break;
						}
					}
				}
			}

			// find incomplete lines
			unsigned long long compscore = 0;
			while (!ignore && !syntaxstack.empty()) {
				auto top = syntaxstack.top();
				syntaxstack.pop();

				compscore = (compscore * 5) + openscoremap.at(top);
			}

			if (compscore) {
				resultvec.push_back(compscore);
			}
		}

		// find middle
		auto middle = resultvec.size() / 2;
		std::nth_element(resultvec.begin(), resultvec.begin() + middle, resultvec.end());
		auto result = resultvec[middle];

		return result;
	}
}

void day10()
{
	// test input
	input("Day10/day10_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day10 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 26397);

	auto p2_test_result = part2();
	std::cout << "[Day10 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 288957);

	// puzzle input
	input("Day10/day10_input");

	auto p1_result = part1();
	std::cout << "[Day10 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 296535);

	auto p2_result = part2();
	std::cout << "[Day10 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 4245130838);

	std::cout << "--------------------------------------------------\n";
}
