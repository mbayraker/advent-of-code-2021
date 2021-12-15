#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <stack>

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
		Cave(size_t n, bool twice) : adj(n), twicevisit(twice) {}

		void addEdge(const Node& n1, const Node& n2) {
			adj[n1.idx].push_back(n2);
			adj[n2.idx].push_back(n1);
		}

		bool twicevisit;
		std::vector<std::vector<Node>> adj;
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

	Cave prepCave(bool twice)
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

		Cave cave{ Node::nodemap.size(), twice };
		for (const auto& path : inputvec) {
			cave.addEdge(Node{path.first}, Node{path.second});
		}

		return cave;
	}

	int dfsCaveIter(Cave& cave, const Node& startNode)
	{
		std::stack<std::tuple<Node, std::string, bool>> s; // node, current path, twice
		int ans = 0;

		s.emplace(startNode, startNode.name, false);

		while (!s.empty()) {
			auto [node, path, twice] = s.top();
			s.pop();

			if (node.name == "end") {
				++ans;
				continue;
			}
			
			for (const auto& nb : cave.adj[node.idx]) {
				if (!nb.islower()) {
					// big cave
					s.emplace(nb, path + nb.name, twice);
				}
				else {
					// small cave
					if (auto pos = path.find(nb.name); pos == std::string::npos) {
						// nb does not exists on the current path
						s.emplace(nb, path + nb.name, twice);
					}
					else if (cave.twicevisit && !twice && nb.name != "start") {
						// visiting small cave twice
						s.emplace(nb, path + nb.name, true);
					}
				}
			}
		}

		return ans;
	}

	int part1()
	{
		Cave cave{ prepCave(false) };
		return dfsCaveIter(cave, Node{ "start" });
	}

	int part2()
	{
		Cave cave{ prepCave(true) };
		return dfsCaveIter(cave, Node{ "start" });
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
