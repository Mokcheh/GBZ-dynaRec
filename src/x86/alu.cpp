#include <x86Emitter.hpp>


using x86 = x86Emitter;


void x86::unary8r(x86_8 r8, unary op)
{
    emitByte(0xFE);
    modRM(mod::RtoR, op , r8);
}

void x86::unary16r(x86_16 r16, unary op)
{
    emitByte(0x66);
    emitByte(0xFE);
    modRM(mod::RtoR, op , r16);
}

void x86::arithmetic8r8imm(x86_8 dest, uint8_t imm8, arithmetic op)
{
    emitByte(0x80);
    modRM(mod::RtoR, op, dest);
    emitByte(imm8);
}



void x86::adc8r8r(x86_8 dest, x86_8 src)
{
    emitByte(0x10);
    modRM(mod::RtoR, src, dest);
}

void x86::add16r16r(x86_16 dest, x86_16 src)
{
    emitByte(0x66);
    emitByte(0x01);
    modRM(mod::RtoR, src, dest);
}

void x86::add8r8r(x86_8 dest, x86_8 src)
{
    emitByte(0x00);
    modRM(mod::RtoR, src, dest);
}


void x86::andr8imm8(x86_8 target, uint8_t imm8)
{
    emitByte(0x80);
    modRM(mod::RtoR, 4, target);
    emitByte(imm8);
}

