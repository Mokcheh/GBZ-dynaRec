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

void x86Emitter::or8stack8r(uint8_t offset, x86_8 src)
{
    emitByte(0x08);
    modRM(mod::RtoMdisp8, src, SIB);
    sib(0, 100, x86_64::RSP);
    emitByte(offset);
}

void x86Emitter::mov8rTo8stack(uint8_t offset, x86_8 src)
{
    emitByte(0x88);
    modRM(mod::RtoMdisp8, src, SIB);
    sib(0, 100, x86_64::RSP);
    emitByte(offset);
}

void x86Emitter::shift8stack(uint8_t units, shift direction)
{
    emitByte(0xC0);
    modRM(mod::RtoMdisp8, (uint8_t)direction, SIB);
    sib(0, 100, x86_64::RSP);
    emitByte(units);
}