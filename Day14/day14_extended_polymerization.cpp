#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <unordered_map>

namespace
{
	std::unordered_map<std::string, char> inputmap;
	std::string polytemp;

	void input(const char* fpath)
	{
		inputmap.clear();
		polytemp.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		std::getline(ifs, polytemp);

		std::string delim{ " -> " };
		for (std::string line{}; std::getline(ifs, line);) {
			if (auto delpos = line.find(delim); delpos != std::string::npos) {
				inputmap.emplace( line.substr(0, delpos),
								  line.substr(delpos + delim.length())[0] );
			}
		}
		ifs.close();
	}

	unsigned long long growPoly(size_t stepSize, std::string poly)
	{
		std::unordered_map<std::string, unsigned long long> polypairs;

		// count the pairs of the starting polymer
		for (size_t i = 0; i < (poly.length() - 1); ++i) {
			++polypairs[poly.substr(i, 2)];
		}

		for (size_t step = 0; step < stepSize; ++step) {
			std::unordered_map<std::string, unsigned long long> newpairs;

			// count the pairs that will generate from the existing pairs
			for (const auto& [pairstr, cnt] : polypairs) {
				std::string new1 = std::string{ pairstr[0] } + inputmap[pairstr];
				std::string new2 = std::string{ inputmap[pairstr] } + pairstr[1];

				newpairs[new1] += cnt;
				newpairs[new2] += cnt;
			}

			// discard the old pairs
			polypairs = std::move(newpairs);
		}

		// count the first char of the each pair
		std::unordered_map<char, unsigned long long> cfreq;
		for (const auto& p : polypairs) {
			cfreq[p.first[0]] += p.second;
		}

		// count the last char
		++cfreq[poly.back()];

		auto f = [](const auto& a, const auto& b) { return a.second < b.second; };
		auto minmax = std::minmax_element(cfreq.cbegin(), cfreq.cend(), f);

		return (minmax.second->second - minmax.first->second);
	}

	unsigned long long part1()
	{
		return growPoly(10, polytemp);
	}

	unsigned long long part2()
	{
		return growPoly(40, polytemp);
	}
}

void day14()
{
	// test input
	input("Day14/day14_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day14 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 1588);

	auto p2_test_result = part2();
	std::cout << "[Day14 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 2188189693529);

	// puzzle input
	input("Day14/day14_input");

	auto p1_result = part1();
	std::cout << "[Day14 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 3284);

	auto p2_result = part2();
	std::cout << "[Day14 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 4302675529689);

	std::cout << "--------------------------------------------------\n";
}
