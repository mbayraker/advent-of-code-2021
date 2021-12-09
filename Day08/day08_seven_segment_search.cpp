#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <array>

namespace
{
	struct Entry {
		std::vector<std::string> notes{};
		std::vector<std::string> output{};
	};

	struct Digit {
		Digit() = default;
		Digit(std::string s1, std::bitset<7> b2) : s{ s1 }, b{ b2 } {}

		std::string s{};
		std::bitset<7> b{};
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

		for (const auto& entry : inputvec) {
			std::array<Digit, 10> digits{};
			std::array<std::bitset<7>, 10> bnotes{};
			std::array<std::bitset<7>, 4> boutputs{};

			// calculate bitmasks for outputs
			for (auto it = entry.output.cbegin(); it != entry.output.cend(); ++it) {
				auto idx = std::distance(entry.output.cbegin(), it);

				for (const auto& c : *it) {
					boutputs[idx].set(c - 'a');
				}
			}

			// find digits 1, 4, 7, 8 (unique len)
			for (auto it = entry.notes.cbegin(); it != entry.notes.cend(); ++it) {
				auto idx = std::distance(entry.notes.cbegin(), it);
				auto len = (*it).length();

				// calculate bitmasks for notes
				for (const auto& c : *it) {
					bnotes[idx].set(c - 'a');
				}

				if (len == 2) {
					digits[1] = Digit{ *it, bnotes[idx] };
				}
				else if (len == 4) {
					digits[4] = Digit{ *it, bnotes[idx] };
				}
				else if (len == 3) {
					digits[7] = Digit{ *it, bnotes[idx] };
				}
				else if (len == 7) {
					digits[8] = Digit{ *it, bnotes[idx] };
				}
			}

			// find 0, 6, 9 (len == 6)
			for (auto it = entry.notes.cbegin(); it != entry.notes.cend(); ++it) {
				auto idx = std::distance(entry.notes.cbegin(), it);

				if ((*it).length() == 6) {
					if ((bnotes[idx] | digits[4].b) == bnotes[idx]) {
						digits[9] = Digit{ *it, bnotes[idx] };
					}
					else if ((bnotes[idx] | digits[1].b) != bnotes[idx]) {
						digits[6] = Digit{ *it, bnotes[idx] };
					}
					else {
						digits[0] = Digit{ *it, bnotes[idx] };
					}
				}
			}

			// find 2, 3, 5 (len == 5)
			for (auto it = entry.notes.cbegin(); it != entry.notes.cend(); ++it) {
				auto idx = std::distance(entry.notes.cbegin(), it);

				if ((*it).length() == 5) {
					if ((bnotes[idx] | digits[1].b) == bnotes[idx]) {
						digits[3] = Digit{ *it, bnotes[idx] };
					}
					else if ((bnotes[idx] | digits[6].b) == digits[6].b) {
						digits[5] = Digit{ *it, bnotes[idx] };
					}
					else {
						digits[2] = Digit{ *it, bnotes[idx] };
					}
				}
			}

			std::string resultstr{};
			for (const auto& bout : boutputs) {
				auto f = [&bout](const auto& digit) {
					return digit.b == bout;
				};

				if (auto it = std::find_if(digits.cbegin(), digits.cend(), f); it != digits.cend()) {
					resultstr += std::to_string(std::distance(digits.cbegin(), it));
				}
			}
			total += std::stoi(resultstr);
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
