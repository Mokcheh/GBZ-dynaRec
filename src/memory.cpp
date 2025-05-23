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

Bus::Bus(std::vector<uint8_t>& testCode):
	romSize(testCode.size())
{
    memory.resize(0x10000);
    for(int i = 0; i < romSize; i++)
        memory[i] = testCode[i];
}
