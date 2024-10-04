#pragma once
#include <cstdint>
#include <vector>
#include <array>

struct Bus {
	Bus(std::vector<uint8_t>& testCode);
	uint8_t readMemory(uint16_t position);
	const uint16_t size;
	std::vector<uint8_t> memory;
};