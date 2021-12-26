#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <functional>

namespace
{
	constexpr int NUM_OF_DIGITS = 14;
	constexpr int CONSTANTS_PER_DIGIT = 10;

	std::array<std::array<int, CONSTANTS_PER_DIGIT>, NUM_OF_DIGITS> constants{};
	
	void input(const char* fpath)
	{
		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (int d = 0; d < NUM_OF_DIGITS; ++d) {
			for (int c = 0; c < CONSTANTS_PER_DIGIT; ++c) {
				for (std::string line{}; std::getline(ifs, line);) {
					if (auto pos = line.find_last_of(' '); pos != std::string::npos) {
						auto operand = line.substr(pos + 1);
						if (operand[0] >= 'w' && operand[0] <= 'z') {
							continue;
						}
						constants[d][c] = std::stoi(operand);
						break;
					}
				}
			}
		}
		ifs.close();
	}

	template<int E, int N>
	struct pow {
		static constexpr long long value = E * pow<E, N - 1>::value;
	};

	template <int E>
	struct pow<E, 0> {
		static constexpr long long value = 1;
	};
	
	long long pow10(int n)
	{
		static constexpr std::array<long long, NUM_OF_DIGITS> lookup = {
			pow<10, 0>::value,
			pow<10, 1>::value,
			pow<10, 2>::value,
			pow<10, 3>::value,
			pow<10, 4>::value,
			pow<10, 5>::value,
			pow<10, 6>::value,
			pow<10, 7>::value,
			pow<10, 8>::value,
			pow<10, 9>::value,
			pow<10, 10>::value,
			pow<10, 11>::value,
			pow<10, 12>::value,
			pow<10, 13>::value
		};

		assert(n < NUM_OF_DIGITS);
		return lookup[n];
	}

	std::array<std::unordered_map<int, long long>, NUM_OF_DIGITS> DP;

	template<int S, int E, typename Comp, typename Next>
	long long solve(int digitIdx, int paramZ)
	{
		// start from most significant digit (digitIdx == 0)
		// end after processing least significant digit (digitIdx == 14)
		if (digitIdx == NUM_OF_DIGITS) {
			if (paramZ == 0) {
				return 0;
			}
			else {
				return -1;
			}
		}

		if (DP[digitIdx].count(paramZ)) {
			return DP[digitIdx][paramZ];
		}
		DP[digitIdx][paramZ] = -1;

		for (int w = S; Comp{}(w, E); w = Next{}(w, 1)) {
			// mul x 0
			// add x z
			int x = paramZ;
			// mod x 26
			if (x < 0) {
				continue;
			}
			x %= 26;
			// div z #VAR
			if (constants[digitIdx][2] == 0) {
				continue;
			}
			int z = paramZ / constants[digitIdx][2];
			// add x #VAR
			x += constants[digitIdx][3];
			// eql x w
			x = (x == w);
			// eql x 0
			x = (x == 0);
			// mul y 0
			// add y 25
			int y = 25;
			// mul y x
			y *= x;
			// add y 1
			++y;
			// mul z y
			z *= y;
			// mul y 0
			// add y w
			y = w;
			// add y #VAR
			y += constants[digitIdx][9];
			// mul y x
			y *= x;
			// add z y
			z += y;

			auto retNum = solve<S, E, Comp, Next>(digitIdx + 1, z);
			if (retNum == -1) {
				continue;
			}
			DP[digitIdx][paramZ] = retNum + (pow10(NUM_OF_DIGITS - 1 - digitIdx) * w);
			return DP[digitIdx][paramZ];
		}

		return DP[digitIdx][paramZ];
	}

	long long part1()
	{
		return solve<9, 1, std::greater_equal<int>, std::minus<int>>(0, 0);
	}

	long long part2()
	{
		for (auto& m : DP) {
			m.clear();
		}

		return solve<1, 9, std::less_equal<int>, std::plus<int>>(0, 0);
	}
}

void day24()
{
	// puzzle input
	input("Day24/day24_input");

	auto p1_result = part1();
	std::cout << "[Day24 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 59998426997979);

	auto p2_result = part2();
	std::cout << "[Day24 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 13621111481315);

	std::cout << "--------------------------------------------------\n";
}
