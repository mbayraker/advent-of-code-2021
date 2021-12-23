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
	std::vector<int> players;

	void input(const char* fpath)
	{
		players.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			if (auto pos = line.find(": "); pos != std::string::npos) {
				players.push_back(std::stoi(line.substr(pos + 2)));
			}
		}
		ifs.close();
	}

	// [pos1, pos2, score1, score2]
	using GameStatus = std::tuple<int, int, int, int>;

	// [p1win, p2win]
	using UniverseCount = std::pair<unsigned long long, unsigned long long>;

	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct GameStatusHasher {
		std::size_t operator()(const GameStatus& t) const {
			std::size_t seed = 0;
			hash_combine(seed, std::get<0>(t));
			hash_combine(seed, std::get<1>(t));
			hash_combine(seed, std::get<2>(t));
			hash_combine(seed, std::get<3>(t));

			return seed;
		}
	};

	// optimization
	std::unordered_map<GameStatus, UniverseCount, GameStatusHasher> DP;

	UniverseCount playDirac(int pos1, int pos2, int score1, int score2)
	{
		if (score1 >= 21) {
			return { 1, 0 };
		}

		if (score2 >= 21) {
			return { 0, 1 };
		}

		if (DP.count({ pos1, pos2, score1, score2 })) {
			return DP[{ pos1, pos2, score1, score2 }];
		}

		UniverseCount result;
		int newpos1, newscore1;

		// play [pos1, score1]
		for (int r1 = 1; r1 <= 3; ++r1) {
			for (int r2 = 1; r2 <= 3; ++r2) {
				for (int r3 = 1; r3 <= 3; ++r3) {
					newpos1 = (pos1 + r1 + r2 + r3) % 10;
					newscore1 = score1 + (newpos1 + 1);

					// play [pos2, score2]
					auto [p2win, p1win] = playDirac(pos2, newpos1, score2, newscore1);

					result.first += p1win;
					result.second += p2win;
				}
			}
		}

		// return [p1win, p2win]
		DP[{ pos1, pos2, score1, score2 }] = result;
		return result;
	}

	struct Dice {
		int roll() {
			val = val % 100;
			++val;
			++count;
			return val;
		}

		int val{};
		size_t count{};
	};

	int part1()
	{
		// use 0 to 9 positions
		int pos1 = players[0] - 1, pos2 = players[1] - 1;
		int score1 = 0, score2 = 0;

		int result = 0;
		Dice dice;

		while (true) {
			pos1 += dice.roll();
			pos1 += dice.roll();
			pos1 += dice.roll();
			pos1 = pos1 % 10;
			score1 += (pos1 + 1); // +1 for 1 to 10 conversion
			if (score1 >= 1000) {
				result = dice.count * score2;
				break;
			}

			pos2 += dice.roll();
			pos2 += dice.roll();
			pos2 += dice.roll();
			pos2 = pos2 % 10;
			score2 += (pos2 + 1); // +1 for 1 to 10 conversion
			if (score2 >= 1000) {
				result = dice.count * score1;
				break;
			}
		}

		return result;
	}

	unsigned long long part2()
	{
		// use 0 to 9 positions
		int pos1 = players[0] - 1, pos2 = players[1] - 1;
		int score1 = 0, score2 = 0;

		auto unicount = playDirac(pos1, pos2, score1, score2);

		return std::max(unicount.first, unicount.second);
	}
}

void day21()
{
	// test input
	input("Day21/day21_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day21 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 739785);

	auto p2_test_result = part2();
	std::cout << "[Day21 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 444356092776315);

	// puzzle input
	input("Day21/day21_input");

	auto p1_result = part1();
	std::cout << "[Day21 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 734820);

	auto p2_result = part2();
	std::cout << "[Day21 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 193170338541590);

	std::cout << "--------------------------------------------------\n";
}
