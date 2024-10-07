#pragma once
#include <cstdint>
#include <vector>
#include <array>

struct Bus {
	Bus(std::vector<uint8_t>& testCode);
	uint8_t readMemory(uint16_t position);
	uint64_t getMemoryAddress(uint16_t position);
	const uint16_t size;
	std::array<uint8_t, 0x4000> rom;
private:
	std::array<uint8_t, 4 * 1024> workRam;
};