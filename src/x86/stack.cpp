#include <x86Emitter.hpp>


void x86Emitter::push16r(x86_16 reg)
{
    emitByte(0x66);
    emitByte(0x50 + reg);
}

void x86Emitter::pop16r(x86_16 reg)
{
    emitByte(0x66);
    emitByte(0x58 + reg);
}

void x86Emitter::orStack8r8(uint8_t offset, x86_8 src)
{
    emitByte(0x08);
    modRM(mod::RtoMdisp8, src, SIB);
    sib(0, 100, RSP);
    emitByte(offset);
}
