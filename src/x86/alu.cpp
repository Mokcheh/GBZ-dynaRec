#include <x86Emitter.hpp>



void x86Emitter::unary8r(x86_8 r8, unary op)
{
    emitByte(0xFE);
    modRM(mod::RtoR, op , r8);
}

void x86Emitter::unary16r(x86_16 r16, unary op)
{
    emitByte(0x66);
    emitByte(0xFE);
    modRM(mod::RtoR, op , r16);
}

void x86Emitter::arithmetic8r8imm(x86_8 dest, uint8_t imm8, arithmetic op)
{
    emitByte(0x80);
    modRM(mod::RtoR, op, dest);
    emitByte(imm8);
}

void x86Emitter::arithmetic8r8r(x86_8 dest, x86_8 src, arithmetic op)
{
    emitByte(0x8 * op);
    modRM(mod::RtoR, src, dest);
}


void x86Emitter::add16r16r(x86_16 dest, x86_16 src)
{
    emitByte(0x66);
    emitByte(0x01);
    modRM(mod::RtoR, src, dest);
}
