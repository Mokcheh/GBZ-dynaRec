#include <x86_64Emitter.hpp>


void x64Emitter::push16r(x86_16 reg)
{
    emitByte(0x66);
    emitByte(0x50 + reg);
}

void x64Emitter::pop16r(x86_16 reg)
{
    emitByte(0x66);
    emitByte(0x58 + reg);
}

void x64Emitter::arithmetic8stack8r(uint8_t offset, x86_8 src, arithmetic op)
{
    emitByte(0x08 * op);
    modRM(mod::RtoMdisp8, src, SIB);
    sib(0, 100, x86_64::RSP);
    emitByte(offset);
}

void x64Emitter::arithmetic8stack8imm(uint8_t offset, uint8_t imm8, arithmetic op)
{
    emitByte(0x80);
    modRM(mod::RtoMdisp8, op, SIB);
    sib(0, 100, x86_64::RSP);
    emitByte(offset);
    emitByte(imm8);
}

void x64Emitter::mov8rTo8stack(uint8_t offset, x86_8 src)
{
    emitByte(0x88);
    modRM(mod::RtoMdisp8, src, SIB);
    sib(0, 100, x86_64::RSP);
    emitByte(offset);
}

void x64Emitter::push64r(x86_64 reg)
{
    emitByte(0x50 + reg);
}

