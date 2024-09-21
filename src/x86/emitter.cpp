#include <x86Emitter.hpp>

x86Emitter::x86Emitter(std::vector<uint8_t>& client): cache(client){}

void x86Emitter::emitByte(uint8_t byte)
{
    cache.emplace_back(byte);
}

void x86Emitter::emitWord(uint16_t word)
{
    cache.emplace_back(word & 0xFF);
    cache.emplace_back(word >> 8);
}

void x86Emitter::modRM(uint8_t mod, uint8_t reg, uint8_t rm)
{
    const uint8_t result = (mod << 6) | (reg << 3) | (rm & 0b111);
    emitByte(result);
}

void x86Emitter::sib(uint8_t scale, uint8_t index, uint8_t base)
{
    const uint8_t result = (scale << 6) | (index << 3) | (base & 0b111);
    emitByte(result);
}
