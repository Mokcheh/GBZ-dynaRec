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

void x86Emitter::arithmetic16r16imm(x86_16 dest, uint16_t imm16, arithmetic op)
{
    emitByte(0x81);
    modRM(mod::RtoR, op, dest);
    emitWord(imm16);
}


void x86Emitter::add16r16r(x86_16 dest, x86_16 src)
{
    emitByte(0x66);
    emitByte(0x01);
    modRM(mod::RtoR, src, dest);
}

void x86Emitter::not8r(x86_8 reg)
{
    emitByte(0xF6);
    modRM(mod::RtoR, 2, reg);
}


void x86Emitter::shift8r(x86_8 target, uint8_t units, shift direction)
{
    emitByte(0xC0);
    modRM(mod::RtoR, (uint8_t)direction, target);
    emitByte(units);
}

void x86Emitter::rotate8r(x86_8 target, uint8_t offset, rotate op)
{
    emitByte(0xC0);
    modRM(mod::RtoR, (uint8_t)op, target);
    emitByte(offset);
}

void x86Emitter::arithmetic64r64r(x86_64 dest, x86_64 src, arithmetic op)
{
    emitByte(0x48);
    emitByte(0x8 * op + 1);
    modRM(mod::RtoR, src, dest);
}

void x86Emitter::arithmetic64r16imm(x86_64 dest, uint16_t imm16, arithmetic op)
{
    emitByte(0x48);
    emitByte(0x83);
    modRM(mod::RtoR, op, dest);
    emitWord(imm16);
}