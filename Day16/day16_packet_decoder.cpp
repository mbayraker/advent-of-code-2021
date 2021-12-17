#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <algorithm>

namespace
{
	std::string inputstr;

	struct Packet {
		Packet(size_t n) : binvec(n, false) {}

		unsigned int getBits(size_t n) {
			if (n > remain()) {
				assert(false);
			}

			unsigned int ret = 0;
			for (size_t i = 0; i < n; ++i) {
				ret = (ret << 1) | binvec[bit_index + i];
			}
			bit_index += n;
			return ret;
		}

		unsigned int remain() const {
			return binvec.size() - bit_index;
		}

		void addVersion(unsigned int vers) {
			version_sum += vers;
		}

		std::vector<bool> binvec;
		unsigned int bit_index{};

		unsigned int version_sum{};
		unsigned long long final_value{};

		// [operation type, in progress, result]
		std::vector<std::tuple<unsigned int, bool, unsigned long long>> opvec{};
	};

	void input(const char* fpath)
	{
		inputstr.clear();

		std::ifstream ifs{ fpath };
		if (!ifs) {
			std::cout << "could not open the file!\n";
			std::exit(EXIT_FAILURE);
		}

		std::getline(ifs, inputstr);

		ifs.close();
	}

	Packet genPacket()
	{
		Packet packet(inputstr.length() * 4);

		int vecidx = 0;
		for (auto c : inputstr) {
			int decval = (c >= 'A') ? (c - 'A' + 10) : (c - '0');
			for (int k = 0; k < 4; ++k) {
				packet.binvec[vecidx + k] = (decval >> (3 - k)) & 0x1;
			}
			vecidx += 4;
		}

		return packet;
	}

	void processSubPacket(Packet& packet);

	void runOperation(Packet& packet, unsigned long long literalVal)
	{
		auto &[op, inprog, value] = packet.opvec.back();

		if (!inprog) {
			value = literalVal;
			inprog = true;
		}
		else {
			switch (op) {
			case 0:
				value += literalVal;
				break;

			case 1:
				value *= literalVal;
				break;

			case 2:
				value = std::min(value, literalVal);
				break;

			case 3:
				value = std::max(value, literalVal);
				break;

			case 5:
				value = (value > literalVal);
				break;

			case 6:
				value = (value < literalVal);
				break;

			case 7:
				value = (value == literalVal);
				break;
			}
		}
	}

	unsigned long long literalPacket(Packet& packet)
	{
		unsigned long long literalVal = 0;

		while (true) {
			auto decval = packet.getBits(5);
			literalVal = (literalVal << 4) | (decval & 0xF);

			if (!(decval & 0x10)) {
				// last group
				break;
			}
		}

		return literalVal;
	}

	void operatorPacket(Packet& packet)
	{
		if (auto lenType = packet.getBits(1); lenType == 0) {
			auto lenTotal = packet.getBits(15);

			while (lenTotal) {
				auto lenPrev = packet.remain();
				processSubPacket(packet);
				lenTotal -= (lenPrev - packet.remain());
			}
		}
		else if (lenType == 1) {
			auto packetNum = packet.getBits(11);

			while (packetNum) {
				processSubPacket(packet);
				--packetNum;
			}
		}
	}

	void processSubPacket(Packet& packet)
	{
		// add version to sum
		packet.addVersion(packet.getBits(3));
		
		// get type
		auto type = packet.getBits(3);

		switch (type) {
		case 4:
			// process literal packet and run the current operation
			runOperation(packet, literalPacket(packet));
			break;

		default:
			// go one operator depth below
			packet.opvec.emplace_back(type, false, 0);

			// process operator packet
			operatorPacket(packet);

			// get current result
			auto result = std::get<2>(packet.opvec.back());

			// go one operator depth above
			packet.opvec.pop_back();

			if (!packet.opvec.empty()) {
				// process the current result with this operator
				runOperation(packet, result);
			}
			else {
				// no operator left, found the final result
				packet.final_value = result;
			}
		}
	}

	unsigned int part1()
	{
		auto packet = genPacket();
		processSubPacket(packet);
		return packet.version_sum;
	}

	unsigned long long part2()
	{
		auto packet = genPacket();
		processSubPacket(packet);
		return packet.final_value;
	}
}

void day16()
{
	// test input
	input("Day16/day16_test_input1");

	auto p1_test_result = part1();
	std::cout << "[Day16 - P1] Test Answer   = " << p1_test_result << "\n";
	assert(p1_test_result == 31);

	// test input
	input("Day16/day16_test_input2");

	auto p2_test_result = part2();
	std::cout << "[Day16 - P2] Test Answer   = " << p2_test_result << "\n";
	assert(p2_test_result == 1);

	// puzzle input
	input("Day16/day16_input");

	auto p1_result = part1();
	std::cout << "[Day16 - P1] Puzzle Answer = " << p1_result << "\n";
	assert(p1_result == 917);

	auto p2_result = part2();
	std::cout << "[Day16 - P2] Puzzle Answer = " << p2_result << "\n";
	assert(p2_result == 2536453523344);

	std::cout << "--------------------------------------------------\n";
}
