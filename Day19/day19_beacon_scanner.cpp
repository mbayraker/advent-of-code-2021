#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <array>
#include <unordered_set>
#include <unordered_map>

namespace
{
	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct Beacon {
		Beacon(int px, int py, int pz) : location{ px, py, pz } {}

		int x() const {
			return location[0];
		}

		int y() const {
			return location[1];
		}

		int z() const {
			return location[2];
		}

		bool operator==(const Beacon& other) const {
			return location == other.location;
		}

		std::array<int, 3> location;
	};

	struct BeaconHasher {
		std::size_t operator()(const Beacon& b) const {
			std::size_t seed = 0;
			hash_combine(seed, b.x());
			hash_combine(seed, b.y());
			hash_combine(seed, b.z());

			return seed;
		}
	};

	struct Scanner {
		std::unordered_set<Beacon, BeaconHasher> beacons;
		std::vector<std::array<int, 3>> positions;
	};

	struct ArrayHasher {
		std::size_t operator()(const std::array<int, 3>& arr) const {
			std::size_t seed = 0;
			hash_combine(seed, arr[0]);
			hash_combine(seed, arr[1]);
			hash_combine(seed, arr[2]);
			return seed;
		}
	};

	std::vector<Scanner> inputvec;
	Scanner mergedscanner;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			if (line.empty()) {
				continue;
			}

			if (auto pos = line.find("---"); pos != std::string::npos) {
				inputvec.emplace_back();
				continue;
			}

			std::replace(line.begin(), line.end(), ',', ' ');
			std::array<int, 3> locs{};
			std::stringstream ss{ line };
			for (int val = 0, cnt = 0; ss >> val; ++cnt) {
				locs[cnt] = val;
			}
			inputvec.back().beacons.emplace(locs[0], locs[1], locs[2]);
		}
		ifs.close();
	}

	template<typename T>
	auto genRot(std::array<T, 3> v)
	{
		auto roll = [](std::array<T, 3> arr) {
			return std::array<T, 3>{ arr[0], arr[2], -arr[1] };
		};

		auto turn = [](std::array<T, 3> arr) {
			return std::array<T, 3>{ -arr[1], arr[0], arr[2] };
		};

		std::vector<std::array<T, 3>> rots;

		for (int cycle = 0; cycle < 2; ++cycle) {
			// RTTT
			for (int step = 0; step < 3; ++step) {
				v = roll(v);
				rots.push_back(v);

				for (int i = 0; i < 3; ++i) {
					v = turn(v);
					rots.push_back(v);
				}
			}
			// RTR
			v = roll(turn(roll(v)));
		}

		return rots;
	}

	bool mergeScanner(Scanner& out, const Scanner& other)
	{
		if (out.beacons.empty()) {
			out.beacons = other.beacons;
			out.positions.push_back(std::array<int, 3>{ 0, 0, 0 });
			return true;
		}

		std::unordered_map<std::array<int, 3>, int, ArrayHasher> delta;

		for (const auto& refb : out.beacons) {
			for (const auto& testb : other.beacons) {
				auto rotsvec = genRot(std::array<int, 3>{ testb.x(), testb.y(), testb.z() });

				for (size_t k = 0; k < rotsvec.size(); ++k) {
					std::array<int, 3> diff{ refb.x() - rotsvec[k][0],
											 refb.y() - rotsvec[k][1],
											 refb.z() - rotsvec[k][2]
					};

					if (++delta[diff] >= 12) {
						// merge beacons
						for (const auto& mergeb : other.beacons) {
							// use the kth rotation
							auto rvec = genRot(std::array<int, 3>{ mergeb.x(), mergeb.y(), mergeb.z() });
							auto x = diff[0] + rvec[k][0];
							auto y = diff[1] + rvec[k][1];
							auto z = diff[2] + rvec[k][2];

							// set will not insert duplicates
							out.beacons.insert(Beacon{ x, y, z });
						}

						out.positions.push_back(diff);
						return true;
					}
				}
			}
		}

		return false;
	}

	size_t findBeacons()
	{
		mergedscanner.beacons.clear();
		mergedscanner.positions.clear();

		std::unordered_set<int> remain;
		for (size_t i = 0; i < inputvec.size(); ++i) {
			remain.insert(i);
		}

		while (!remain.empty()) {
			for (size_t i = 0; i < inputvec.size(); ++i) {
				if (!remain.count(i)) {
					continue;
				}

				if (mergeScanner(mergedscanner, inputvec[i])) {
					remain.erase(i);
				}
			}
		}

		return mergedscanner.beacons.size();
	}

	int part1()
	{
		return findBeacons();
	}

	int part2()
	{
		if (mergedscanner.positions.empty()) {
			findBeacons();
		}

		int manhattan = std::numeric_limits<int>::min();

		for (size_t a = 0; a < mergedscanner.positions.size(); ++a) {
			for (size_t b = 0; b < mergedscanner.positions.size(); ++b) {
				auto xdiff = std::abs(mergedscanner.positions[a][0] - mergedscanner.positions[b][0]);
				auto ydiff = std::abs(mergedscanner.positions[a][1] - mergedscanner.positions[b][1]);
				auto zdiff = std::abs(mergedscanner.positions[a][2] - mergedscanner.positions[b][2]);
				manhattan = std::max(manhattan, xdiff + ydiff + zdiff);
			}
		}

		return manhattan;
	}
}

void day19()
{
	// test input
	input("Day19/day19_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day19 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 79);

	auto p2_test_result = part2();
	std::cout << "[Day19 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 3621);

	// puzzle input
	input("Day19/day19_input");

	auto p1_result = part1();
	std::cout << "[Day19 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 419);

	auto p2_result = part2();
	std::cout << "[Day19 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 13210);

	std::cout << "--------------------------------------------------\n";
}
