#include <x86Emitter.hpp>


void x86Emitter::mov8immTo8r(x86_8 r8, uint8_t imm8)
{
    emitByte(0xB0 + r8);
    emitByte(imm8);
}

void x86Emitter::mov16immTo16r(x86_16 r16, uint16_t imm16)
{
    emitByte(0x66);
    emitByte(0xB8 + r16);
    emitWord(imm16);
}

void x86Emitter::mov16rTo16r(x86_16 dest, x86_16 src)
{
    emitByte(0x66);
    emitByte(0x89);
    modRM(mod::RtoR, src, dest);
}

void x86Emitter::mov8rTo8r(x86_8 dest, x86_8 src)
{
    emitByte(0x88);
    modRM(mod::RtoR, src, dest);
}
