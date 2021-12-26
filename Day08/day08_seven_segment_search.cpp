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
	struct Entry {
		std::vector<std::string> notes{};
		std::vector<std::string> output{};
	};

	std::vector<Entry> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			Entry e{};
			std::stringstream ss{ line };
			for (int i = 0; i < 11; ++i) {
				std::string valstr{};
				ss >> valstr;
				if (valstr == "|") {
					for (int j = 0; j < 4; ++j) {
						ss >> valstr;
						e.output.push_back(valstr);
					}
				}
				else {
					e.notes.push_back(valstr);
				}
			}
			inputvec.push_back(e);
		}
		ifs.close();
	}

	int part1()
	{
		int result = 0;
		for (const auto& entry : inputvec) {
			auto f = [](const auto& s) {
				// cf (1), bcdf (4), acf (7), abcdefg (8)
				return (s.length() == 2) || (s.length() == 4) ||
					   (s.length() == 3) || (s.length() == 7);
			};

			result += std::count_if(entry.output.cbegin(), entry.output.cend(), f);
		}

		return result;
	}

	int part2()
	{
		int total = 0;

		// scores for each character:
		// { {'a', 8}, {'b', 6}, {'c', 8}, {'d', 7}, {'e', 4}, {'f', 9}, {'g', 7} }

		// scores for each digit:
		const std::map<int, int> digitmap {
			{42, 0}, // abcefg
			{17, 1}, // cf
			{34, 2}, // acdeg
			{39, 3}, // acdfg
			{30, 4}, // bcdf
			{37, 5}, // abdfg
			{41, 6}, // abdefg
			{25, 7}, // acf
			{49, 8}, // abcdefg
			{45, 9}  // abcdfg
		};

		for (const auto& entry : inputvec) {
			// calculate the score for each character in the notes
			std::map<char, int> segmap{};
			for (const auto& note : entry.notes) {
				for (const auto& c : note) {
					segmap[c]++;
				}
			}

			// calculate the score for each digit in the outputs and find the result
			std::string result{};
			for (const auto& out : entry.output) {
				int sum = 0;
				for (const auto& c : out) {
					sum += segmap[c];
				}
				result += std::to_string(digitmap.at(sum));
			}

			total += std::stoi(result);
		}

		return total;
	}
}

void day08()
{
	// test input
	input("Day08/day08_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day08 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 26);

	auto p2_test_result = part2();
	std::cout << "[Day08 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 61229);

	// puzzle input
	input("Day08/day08_input");

	auto p1_result = part1();
	std::cout << "[Day08 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 288);

	auto p2_result = part2();
	std::cout << "[Day08 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 940724);

	std::cout << "--------------------------------------------------\n";
}
