#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace
{
	using Board = std::vector<std::string>;
	using BPair = std::pair<size_t, Board>;

	const std::unordered_map<char, size_t> roomPos = {
		{'A', 2}, {'B', 4}, {'C', 6}, {'D', 8}
	};

	const std::unordered_set<size_t> hallwayPos = {
		0, 1, 3, 5, 7, 9, 10
	};

	const std::unordered_map<char, size_t> moveCost = {
		{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}
	};

	std::vector<char> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			for (auto c : line) {
				if (std::isalpha(c)) {
					inputvec.push_back(c);
				}
			}
		}
		ifs.close();
	}

	template <typename T>
	inline void hash_combine(std::size_t& seed, const T& v) {
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct BoardHasher {
		std::size_t operator()(const Board& board) const {
			std::size_t seed = 0;
			for (const auto& str : board) {
				hash_combine(seed, str);
			}
			return seed;
		}
	};

	auto prepBoard(bool folded)
	{
		// represent each column as a string (total of 11)
		Board board(11);
		std::array<size_t, 4> idxarr{ 2, 4, 6, 8 };

		// construct room columns without leading '.'
		auto idxitr = idxarr.begin();
		for (size_t i = 0; i < inputvec.size(); ++i) {
			board[*idxitr] += inputvec[i];
			if (++idxitr == idxarr.end()) {
				idxitr = idxarr.begin();
			}
		}

		// construct hallway columns with a single '.'
		for (size_t i = 0; i < board.size(); ++i) {
			if (board[i].empty()) {
				board[i] = ".";
			}
		}

		if (!folded) {
			std::array<std::string, 4> str = { "DD", "CB", "BA", "AC" };
			size_t stridx = 0;
			for (const auto idx : idxarr) {
				board[idx].insert(1, str[stridx]);
				stridx++;
			}
		}

		return board;
	}

	auto prepDestBoard(size_t depth)
	{
		Board board(11);

		for (const auto& p : roomPos) {
			for (size_t i = 0; i < depth; ++i) {
				board[p.second] += p.first;
			}
		}

		for (size_t i = 0; i < board.size(); ++i) {
			if (board[i].empty()) {
				board[i] = ".";
			}
		}

		return board;
	}

	char getAmphipod(const std::string roomStr)
	{
		// return the first amphipod from the room
		for (const auto c : roomStr) {
			if (c != '.') {
				return c;
			}
		}

		return 0;
	}

	bool isReachable(const Board& board, size_t start, size_t end)
	{
		// is there any amphipods between start and end?
		// if there is not, return true
		for (auto pos = std::min(start, end); pos <= std::max(start, end); ++pos) {
			// consider only hallway positions
			if (pos == start || !hallwayPos.count(pos)) {
				continue;
			}

			if (board[pos][0] != '.') {
				return false;
			}
		}

		return true;
	}

	bool isRoomOK(const std::string roomStr, char amph)
	{
		// if the room contains no other amphipods, return true
		std::unordered_map<char, size_t> cnts;
		for (auto c : roomStr) {
			++cnts[c];
		}

		return roomStr.size() == cnts['.'] + cnts[amph];
	}

	std::vector<size_t> genMoves(const Board& board, size_t pos)
	{
		auto roomStr = board[pos];
		auto amph = getAmphipod(roomStr);

		// if the amphipod is in its destination room and that room is OK
		if (pos == roomPos.at(amph) && isRoomOK(roomStr, amph)) {
			return {}; // no move
		}

		// if the amphipod can move to its destination room and that room is OK
		if (isReachable(board, pos, roomPos.at(amph)) &&
			isRoomOK(board[roomPos.at(amph)], amph)) {
			return std::vector<size_t>{ roomPos.at(amph) }; // move to the room
		}

		// if amphipod is in hallway and can't move to its destination room
		if (hallwayPos.count(pos)) {
			return {}; // no move
		}

		// amphipod is in another room: generate possible hallway moves
		std::vector<size_t> movevec;
		for (size_t destpos = 0; destpos < board.size(); ++destpos) {
			if (destpos == pos || !hallwayPos.count(destpos)) {
				continue;
			}

			if (isReachable(board, pos, destpos)) {
				movevec.push_back(destpos);
			}
		}

		return movevec;
	}

	BPair execMove(const Board& board, int src, int dest)
	{
		Board newboard(board);
		size_t steps = 0;
		auto amph = getAmphipod(newboard[src]);

		if (hallwayPos.count(src)) {
			// from hallway
			newboard[src] = ".";
		}
		else {
			// from room
			if (auto a = newboard[src].find_first_not_of('.'); a != std::string::npos) {
				newboard[src][a] = '.';
				steps += (a + 1);
			}
		}

		steps += std::abs(dest - src);

		if (hallwayPos.count(dest)) {
			// to hallway
			newboard[dest] = std::string{ amph };
		}
		else {
			// to room
			if (auto dot = newboard[dest].find_last_of('.'); dot != std::string::npos) {
				newboard[dest][dot] = amph;
				steps += (dot + 1);
			}
		}

		// calculate cost
		size_t cost = steps * moveCost.at(amph);
		return std::make_pair(cost, newboard);
	}

	size_t organizeBoard(Board& startBoard, const Board& destBoard)
	{
		// dijkstra
		std::priority_queue<BPair, std::vector<BPair>, std::greater<BPair>> pque; // min heap
		std::unordered_map<Board, size_t, BoardHasher> costs;

		pque.emplace(0, startBoard);

		while (!pque.empty()) {
			auto [currCost, currBoard] = pque.top();
			pque.pop();

			// consider every amphipod
			for (size_t pos = 0; pos < currBoard.size(); ++pos) {
				if (getAmphipod(currBoard[pos]) == 0) {
					continue;
				}

				auto destvec = genMoves(currBoard, pos);
				for (const auto dest : destvec) {
					auto [extraCost, newBoard] = execMove(currBoard, pos, dest);
					auto newCost = currCost + extraCost;

					if (!costs.count(newBoard) || newCost < costs[newBoard]) {
						costs[newBoard] = newCost;
						pque.emplace(newCost, newBoard);
					}
				}
			}
		}

		return costs.at(destBoard);
	}

	size_t part1()
	{
		auto startboard = prepBoard(true);
		auto destboard = prepDestBoard(2);
		return organizeBoard(startboard, destboard);
	}

	size_t part2()
	{
		auto startboard = prepBoard(false);
		auto destboard = prepDestBoard(4);
		return organizeBoard(startboard, destboard);
	}
}

void day23()
{
	// test input
	input("Day23/day23_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day23 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 12521);

	auto p2_test_result = part2();
	std::cout << "[Day23 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 44169);

	// puzzle input
	input("Day23/day23_input");

	auto p1_result = part1();
	std::cout << "[Day23 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 15472);

	auto p2_result = part2();
	std::cout << "[Day23 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 46182);

	std::cout << "--------------------------------------------------\n";
}
