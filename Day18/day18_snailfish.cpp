#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>

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

	std::string addNums(const std::string& rstr, const std::string& lstr)
	{
		return std::string{ '[' } + rstr + ',' + lstr + ']';
	}

	void explodeNum(std::string& exp, size_t pos)
	{
		const std::string digitstr{ "0123456789" };

		size_t startPos = pos;
		size_t endPos = exp.find(']', startPos);

		// the pair's left value is added to
		// the first regular number to the left of the exploding pair (if any)
		// search [begin(), startPos]
		if (auto lpos = exp.find_last_of(digitstr, startPos); lpos != std::string::npos) {
			// handle multidigit
			size_t prevNumOfDigit = 1;
			while (std::isdigit(exp[lpos - 1])) {
				--lpos;
				++prevNumOfDigit;
			}

			size_t currNumOfDigit = exp.find(',', startPos) - (startPos + 1);

			auto newl_str = std::to_string(
				std::stoi(exp.substr(lpos, prevNumOfDigit)) +
				std::stoi(exp.substr(startPos + 1, currNumOfDigit))
			);

			if (prevNumOfDigit == newl_str.length()) {
				exp.replace(lpos, newl_str.length(), newl_str);
			}
			else {
				exp = exp.substr(0, lpos) + newl_str + exp.substr(lpos + prevNumOfDigit);
				startPos += (newl_str.length() - prevNumOfDigit);
				endPos += (newl_str.length() - prevNumOfDigit);
			}
		}

		// the pair's right value is added to
		// the first regular number to the right of the exploding pair (if any)
		// search [endPos, end())
		if (auto rpos = exp.find_first_of(digitstr, endPos); rpos != std::string::npos) {
			// handle multidigit
			size_t prevNumOfDigit = 1;
			while (std::isdigit(exp[rpos + prevNumOfDigit])) {
				++prevNumOfDigit;
			}

			auto currPos = exp.find(',', startPos) + 1;
			size_t currNumOfDigit = endPos - currPos;

			auto newr_str = std::to_string(
				std::stoi(exp.substr(rpos, prevNumOfDigit)) +
				std::stoi(exp.substr(currPos, currNumOfDigit))
			);

			if (prevNumOfDigit == newr_str.length()) {
				exp.replace(rpos, newr_str.length(), newr_str);
			}
			else {
				exp = exp.substr(0, rpos) + newr_str + exp.substr(rpos + prevNumOfDigit);
			}
		}

		// exploding pair is replaced with the regular number 0
		exp.replace(startPos, (endPos - startPos + 1), "0");
	}

	void splitNum(std::string& str, size_t pos)
	{
		// left  : regular number divided by two and rounded down
		// right : regular number divided by two and rounded up

		size_t prevNumOfDigit = 1;
		while (std::isdigit(str[pos + prevNumOfDigit])) {
			++prevNumOfDigit;
		}

		int num = std::stoi(str.substr(pos, prevNumOfDigit));

		int lnum = num / 2;
		int rnum = static_cast<int>(std::ceil(num / 2.0));

		auto newstr = std::string{ '[' } + std::to_string(lnum) + ',' +
			std::to_string(rnum) + ']';

		str = str.substr(0, pos) + newstr + str.substr(pos + prevNumOfDigit);
	}

	bool reduceNum(std::string& reducedstr, size_t pos)
	{
		// 1st: explode
		std::vector<char> indent;
		size_t n = pos;
		size_t N = reducedstr.length();

		while (n < N) {
			if (reducedstr[n] == '[') {
				indent.push_back(reducedstr[n]);

				if (indent.size() > 4) {
					explodeNum(reducedstr, n);
					return true;
				}
			}
			else if (reducedstr[n] == ']') {
				indent.pop_back();
			}
			++n;
		}

		// 2nd: split
		// search for numbers 10 or greater
		auto spiter = std::adjacent_find(reducedstr.cbegin(), reducedstr.cend(),
			[](const auto& a, const auto& b) {
				return std::isdigit(a) && std::isdigit(b);
			});

		if (spiter != reducedstr.cend()) {
			splitNum(reducedstr, std::distance(reducedstr.cbegin(), spiter));
			return true;
		}

		return false;
	}

	int calcMagnitude(const std::string& str)
	{
		std::vector<std::vector<int>> mag(1);

		for (size_t n = 0; n < str.length(); ++n) {
			if (str[n] == '[') {
				mag.emplace_back();
			}
			else if (str[n] == ']') {
				auto m = (3 * mag.back()[0]) + (2 * mag.back()[1]);
				mag.pop_back();
				mag.back().push_back(m);
			}
			else if (std::isdigit(str[n])) {
				mag.back().push_back(str[n] - '0');
			}
		}

		return mag[0][0];
	}

	int part1()
	{
		std::string finalsum;

		for (auto iter = inputvec.cbegin(); iter != inputvec.cend(); ++iter) {
			if (iter == inputvec.cbegin()) {
				finalsum = *iter;
			}
			else {
				finalsum = addNums(finalsum, *iter);
				while (reduceNum(finalsum, 0));
			}
		}

		return calcMagnitude(finalsum);
	}

	int part2()
	{
		int maxMag = -1;

		for (size_t i = 0; i < inputvec.size(); ++i) {
			for (size_t j = 0; j < inputvec.size(); ++j) {
				if (i == j) {
					continue;
				}

				std::string sumstr{ inputvec[i] };
				sumstr = addNums(sumstr, inputvec[j]);
				while (reduceNum(sumstr, 0));

				maxMag = std::max(maxMag, calcMagnitude(sumstr));
			}
		}

		return maxMag;
	}
}

void day18()
{
	// test input
	input("Day18/day18_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day18 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 4140);

	auto p2_test_result = part2();
	std::cout << "[Day18 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 3993);

	// puzzle input
	input("Day18/day18_input");

	auto p1_result = part1();
	std::cout << "[Day18 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 3869);

	auto p2_result = part2();
	std::cout << "[Day18 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 4671);

	std::cout << "--------------------------------------------------\n";
}
