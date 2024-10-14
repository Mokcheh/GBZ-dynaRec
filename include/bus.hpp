#pragma once
#include <cstdint>
#include <vector>

struct Bus {
	Bus(std::vector<uint8_t>& testCode);
	const uint16_t romSize;
	std::vector<uint8_t> memory;
};