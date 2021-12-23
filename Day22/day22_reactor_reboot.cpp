#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <array>
#include <unordered_map>

namespace
{
	using Cube = std::tuple<int, int, int, int, int, int>;

	struct Step {
		Step(bool on, int x1, int x2, int y1, int y2, int z1, int z2) :
			status(on), cube(x1, x2, y1, y2, z1, z2) {}

		bool status;
		Cube cube;
	};

	std::vector<Step> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			bool status = false;
			if (auto posblank = line.find(' '); posblank != std::string::npos) {
				if (line.substr(0, posblank) == "on") {
					status = true;
				}
			}

			std::array<std::pair<int, int>, 3> loc{};
			size_t offs = 0;
			for (size_t i = 0; i < 3; ++i) {
				if (auto posmin = line.find('=', offs); posmin != std::string::npos) {
					offs = posmin + 1;
					loc[i].first = std::stoi(line.substr(offs));

					if (auto posmax = line.find("..", offs); posmax != std::string::npos) {
						offs = posmax + 2;
						loc[i].second = std::stoi(line.substr(offs));
					}
				}
			}

			inputvec.emplace_back(status, loc[0].first, loc[0].second,
										  loc[1].first, loc[1].second,
										  loc[2].first, loc[2].second);
		}
		ifs.close();
	}

	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct CubeHasher {
		std::size_t operator()(const Cube& t) const {
			std::size_t seed = 0;
			hash_combine(seed, std::get<0>(t));
			hash_combine(seed, std::get<1>(t));
			hash_combine(seed, std::get<2>(t));
			hash_combine(seed, std::get<3>(t));
			hash_combine(seed, std::get<4>(t));
			hash_combine(seed, std::get<5>(t));

			return seed;
		}
	};

	long long reboot(bool small)
	{
		std::unordered_map<Cube, int, CubeHasher> reactor;
		for (const auto& s : inputvec) {
			auto &[x1, x2, y1, y2, z1, z2] = s.cube;

			if (small && (x1 < -50 || x2 > 50 ||
				          y1 < -50 || y2 > 50 ||
				          z1 < -50 || z2 > 50)) {
				continue;
			}

			// remove intersection (add with negative sign)
			std::unordered_map<Cube, int, CubeHasher> intersection;
			for (const auto& [rcube, rcount] : reactor) {
				auto &[rx1, rx2, ry1, ry2, rz1, rz2] = rcube;

				auto int_x1 = std::max(rx1, x1);
				auto int_x2 = std::min(rx2, x2);

				auto int_y1 = std::max(ry1, y1);
				auto int_y2 = std::min(ry2, y2);

				auto int_z1 = std::max(rz1, z1);
				auto int_z2 = std::min(rz2, z2);

				if (int_x1 <= int_x2 && int_y1 <= int_y2 && int_z1 <= int_z2) {
					intersection[{ int_x1, int_x2, int_y1, int_y2, int_z1, int_z2 }] -= rcount;
				}
			}

			// copy intersection to the reactor
			for (const auto& [icube, icount] : intersection) {
				reactor[icube] += icount;
			}

			// add "on" cubes
			if (s.status) {
				++reactor[s.cube];
			}
		}

		long long result = 0;
		for (const auto& [rcube, rcount] : reactor) {
			auto &[rx1, rx2, ry1, ry2, rz1, rz2] = rcube;

			result += ((long long)(rx2 - rx1 + 1) *
					   (long long)(ry2 - ry1 + 1) *
					   (long long)(rz2 - rz1 + 1) *
					   (long long)(rcount));
		}

		return result;
	}

	long long part1()
	{
		return reboot(true);
	}

	long long part2()
	{
		return reboot(false);
	}
}

void day22()
{
	// test input
	input("Day22/day22_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day22 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 590784);

	// test input
	input("Day22/day22_test_input2");

	auto p2_test_result = part2();
	std::cout << "[Day22 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 2758514936282235);

	// puzzle input
	input("Day22/day22_input");

	auto p1_result = part1();
	std::cout << "[Day22 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 615700);

	auto p2_result = part2();
	std::cout << "[Day22 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 1236463892941356);

	std::cout << "--------------------------------------------------\n";
}
