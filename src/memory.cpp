#include <bus.hpp>

namespace gbz80Address
{
	namespace rom
	{
		const uint16_t start = 0;
		const uint16_t end = 0x3FFF;
	}
	namespace workRam
	{
		const uint16_t start = 0xC000;
		const uint16_t end = 0xCFFF;
	}
};

Bus::Bus(std::vector<uint8_t>& testCode) :
	size(testCode.size())
{
	memory.resize(0x10000);
	/*
	for (auto opcode: testCode)
		memory.emplace_back(opcode);*/
	for (int i = 0; i < size; i++)
		memory[i] = testCode[i];
}

uint8_t Bus::readMemory(uint16_t position)
{
	return memory[position];
}