#include <x86Emitter.hpp>

x86Emitter::x86Emitter(std::vector<uint8_t>& client): cache(client){}

void x86Emitter::mov(x86_8 Register, uint8_t memory)
{

}

void x86Emitter::mov(x86_8 destinationReg, x86_8 sourceReg)
{

}

void x86Emitter::mov(x86_16 Register, uint16_t memory)
{
    emitByte(0x66);
    emitByte(0x89);
    /*uncomplete*/
}

void x86Emitter::emitByte(uint8_t byte)
{
    cache.emplace_back(byte);
}
