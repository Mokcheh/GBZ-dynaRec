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
	workRam.fill(0);
	for (int i = 0; i < size; i++)
		rom[i] = testCode[i];
}

uint8_t Bus::readMemory(uint16_t position)
{
	return rom[position];
}

uint64_t Bus::getMemoryAddress(uint16_t position)
{
	if (position >= gbz80Address::workRam::start && position <= gbz80Address::workRam::end)
		return workRam[position - gbz80Address::workRam::start];
	return 0;
}