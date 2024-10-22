#include <x86_64Emitter.hpp>



void x64Emitter::unary8r(x86_8 r8, unary op)
{
    emitByte(0xFE);
    modRM(mod::RtoR, op , r8);
}

void x64Emitter::unary16r(x86_16 r16, unary op)
{
    emitByte(0x66);
    emitByte(0xFE);
    modRM(mod::RtoR, op , r16);
}

void x64Emitter::arithmetic8r8imm(x86_8 dest, uint8_t imm8, arithmetic op)
{
    emitByte(0x80);
    modRM(mod::RtoR, op, dest);
    emitByte(imm8);
}

void x64Emitter::arithmetic8r8r(x86_8 dest, x86_8 src, arithmetic op)
{
    emitByte(0x8 * op);
    modRM(mod::RtoR, src, dest);
}

void x64Emitter::arithmetic16r16imm(x86_16 dest, uint16_t imm16, arithmetic op)
{
    emitByte(0x66);
    emitByte(0x81);
    modRM(mod::RtoR, op, dest);
    emitWord(imm16);
}


void x64Emitter::add16r16r(x86_16 dest, x86_16 src)
{
    emitByte(0x66);
    emitByte(0x01);
    modRM(mod::RtoR, src, dest);
}

void x64Emitter::not8r(x86_8 reg)
{
    emitByte(0xF6);
    modRM(mod::RtoR, 2, reg);
}

void x64Emitter::arithmetic64r64r(x86_64 dest, x86_64 src, arithmetic op)
{
    emitByte(0x48);
    emitByte(0x8 * op + 1);
    modRM(mod::RtoR, src, dest);
}

void x64Emitter::arithmetic64r64imm(x86_64 dest, uint64_t imm64, arithmetic op)
{
    emitByte(0x48);
    emitByte(0x81);
    modRM(mod::RtoR, op, dest);
    emitQWord(imm64);
}

void x64Emitter::bitwise8r(x86_8 target, uint8_t units, uint8_t op)
{
    if(op > (uint8_t)shift::RIGHT)
        return;
    emitByte(0xC0);
    modRM(mod::RtoR, op, target);
    emitByte(units);
}

void x64Emitter::bitwise8r(x86_8 target, uint8_t op)
{
    if(op > (uint8_t)shift::RIGHT)
        return;
    emitByte(0xD0);
    modRM(mod::RtoR, op, 0);
}
