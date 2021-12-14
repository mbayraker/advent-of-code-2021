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
	struct Node {
		Node(std::string s) : name(s), idx(nodemap[s]) {}

		bool islower() const {
			return std::islower(name[0]);
		}

		std::string name;
		int idx;
		static std::unordered_map<std::string, int> nodemap;
	};

	std::unordered_map<std::string, int> Node::nodemap;

	struct Cave {
		Cave(size_t n) : adj(n), twicevisit(false) {}

		void addEdge(const Node& n1, const Node& n2) {
			adj[n1.idx].push_back(n2);
			adj[n2.idx].push_back(n1);
		}

		bool twicevisit;
		std::vector<std::vector<Node>> adj;
		std::vector<std::vector<std::string>> fullpaths;
	};

	std::vector<std::pair<std::string, std::string>> inputvec;

	void input(const char* fpath)
	{
		inputvec.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		for (std::string line{}; std::getline(ifs, line);) {
			if (auto delpos = line.find('-'); delpos != std::string::npos) {
				inputvec.emplace_back(line.substr(0, delpos),
                                      line.substr(delpos + 1));
			}
		}
		ifs.close();
	}

	Cave prepCave()
	{
		Node::nodemap.clear();

		for (const auto& path : inputvec) {
			if (!Node::nodemap.count(path.first)) {
				Node::nodemap[path.first] = Node::nodemap.size();
			}
			if (!Node::nodemap.count(path.second)) {
				Node::nodemap[path.second] = Node::nodemap.size();
			}
		}

		Cave cave{ Node::nodemap.size() };
		for (const auto& path : inputvec) {
			cave.addEdge(Node{path.first}, Node{path.second});
		}

		return cave;
	}

	void dfsCave(Cave& cave, const Node& currNode, std::vector<std::string>& currPath,
		std::vector<bool>& visited, int& twiceidx)
	{
		if (currNode.islower()) {
			visited[currNode.idx] = true;
		}

		currPath.push_back(currNode.name);

		if (currNode.name == "end") {
			cave.fullpaths.push_back(currPath);
		}
		else {
			for (const auto& nb : cave.adj[currNode.idx]) {
				if (nb.name == "start") {
					continue;
				}

				if (cave.twicevisit && (twiceidx < 0) && nb.islower() && visited[nb.idx]) {
					// first time visiting a small cave twice
					twiceidx = nb.idx;
					dfsCave(cave, nb, currPath, visited, twiceidx);
					continue;
				}

				if (!visited[nb.idx]) {
					dfsCave(cave, nb, currPath, visited, twiceidx);
				}
			}
		}

		currPath.pop_back();
		if (cave.twicevisit && (twiceidx == currNode.idx)) {
			twiceidx = -1;
		}
		else {
			visited[currNode.idx] = false;
		}
	}

	int part1()
	{
		Cave cave{ prepCave() };

		std::vector<bool> visited(Node::nodemap.size(), false);
		std::vector<std::string> path;
		int idx{ -1 };
		
		dfsCave(cave, Node{ "start" }, path, visited, idx);
		return cave.fullpaths.size();
	}

	int part2()
	{
		Cave cave{ prepCave() };
		cave.twicevisit = true;

		std::vector<bool> visited(Node::nodemap.size(), false);
		std::vector<std::string> path;
		int idx{ -1 };

		dfsCave(cave, Node{ "start" }, path, visited, idx);
		return cave.fullpaths.size();
	}
}

void day12()
{
	// test input
	input("Day12/day12_test_input");

	auto p1_test_result = part1();
	std::cout << "[Day12 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 10);

	auto p2_test_result = part2();
	std::cout << "[Day12 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 36);

	// puzzle input
	input("Day12/day12_input");

	auto p1_result = part1();
	std::cout << "[Day12 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 3779);

	auto p2_result = part2();
	std::cout << "[Day12 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 96988);

	std::cout << "--------------------------------------------------\n";
}
