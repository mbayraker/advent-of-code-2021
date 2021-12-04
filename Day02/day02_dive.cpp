#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
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
		const std::string delimiter{ " " };
		std::map<std::string, int> posmap;

		for (const auto& s : inputvec) {
			if (auto delpos = s.find(delimiter); delpos != std::string::npos) {
				auto dir = s.substr(0, delpos);
				auto val = std::stoi(s.substr(delpos + delimiter.length()));

				posmap[dir] += val;
			}
		}

		auto result = posmap["forward"] * (posmap["down"] - posmap["up"]);
		return result;
	}

	int part2()
	{
		const std::string delimiter{ " " };
		int aim = 0, depth = 0, horizontal = 0;

		for (const auto& s : inputvec) {
			if (auto delpos = s.find(delimiter); delpos != std::string::npos) {
				auto dir = s.substr(0, delpos);
				auto val = std::stoi(s.substr(delpos + delimiter.length()));

				if (dir == "down") {
					aim += val;
				}
				else if (dir == "up") {
					aim -= val;
				}
				else if (dir == "forward") {
					horizontal += val;
					depth += (aim * val);
				}
			}
		}

		auto result = horizontal * depth;
		return result;
	}
}

void day02()
{
	// test input
	input("Day02/day02_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day02 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 150);

	auto p2_test_result = part2();
	std::cout << "[Day02 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 900);

	// puzzle input
	input("Day02/day02_input");
	
	auto p1_result = part1();
	std::cout << "[Day02 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 2027977);

	auto p2_result = part2();
	std::cout << "[Day02 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1903644897);

	std::cout << "--------------------------------------------------\n";
}
