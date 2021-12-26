#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cassert>

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

	int part1()
	{
		std::map<char, int> bitcountmap;
		std::string gamma{}, epsilon{};

		auto columnsize = inputvec[0].size();

		for (size_t pos = 0; pos < columnsize; ++pos) {
			for (const auto& s : inputvec) {
				bitcountmap[s[pos]]++;
			}

			if (bitcountmap['0'] > bitcountmap['1']) {
				gamma += '0';
				epsilon += '1';
			}
			else {
				gamma += '1';
				epsilon += '0';
			}

			bitcountmap.clear();
		}

		int result = stoi(gamma, 0, 2) * stoi(epsilon, 0, 2);
		return result;
	}

	int part2()
	{
		std::map<char, int> bitcountmap;
		std::string oxygen{}, co2{};

		std::vector<std::string> testvec{ inputvec };
		auto columnsize = testvec[0].size();

		// oxygen generator rating
		for (size_t pos = 0; pos < columnsize; ++pos) {
			if (testvec.size() == 1) {
				oxygen += testvec[0][pos];
				continue;
			}

			for (const auto& s : testvec) {
				bitcountmap[s[pos]]++;
			}

			if (bitcountmap['0'] > bitcountmap['1']) {
				oxygen += '0';
				auto fcomp = [pos](const auto& str) { return str[pos] == '1'; };
				testvec.erase(std::remove_if(testvec.begin(), testvec.end(), fcomp), testvec.end());
			}
			else {
				oxygen += '1';
				auto fcomp = [pos](const auto& str) { return str[pos] == '0'; };
				testvec.erase(std::remove_if(testvec.begin(), testvec.end(), fcomp), testvec.end());
			}

			bitcountmap.clear();
		}

		testvec = inputvec;

		// co2 scrubber rating
		for (size_t pos = 0; pos < columnsize; ++pos) {
			if (testvec.size() == 1) {
				co2 += testvec[0][pos];
				continue;
			}

			for (const auto& s : testvec) {
				bitcountmap[s[pos]]++;
			}

			if (bitcountmap['0'] > bitcountmap['1']) {
				co2 += '1';
				auto fcomp = [pos](const auto& str) { return str[pos] == '0'; };
				testvec.erase(std::remove_if(testvec.begin(), testvec.end(), fcomp), testvec.end());
			}
			else {
				co2 += '0';
				auto fcomp = [pos](const auto& str) { return str[pos] == '1'; };
				testvec.erase(std::remove_if(testvec.begin(), testvec.end(), fcomp), testvec.end());
			}

			bitcountmap.clear();
		}

		int result = stoi(oxygen, 0, 2) * stoi(co2, 0, 2);
		return result;
	}
}

void day03()
{
	// test input
	input("Day03/day03_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day03 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 198);

	auto p2_test_result = part2();
	std::cout << "[Day03 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 230);

	// puzzle input
	input("Day03/day03_input");

	auto p1_result = part1();
	std::cout << "[Day03 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 1082324);

	auto p2_result = part2();
	std::cout << "[Day03 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1353024);

	std::cout << "--------------------------------------------------\n";
}
