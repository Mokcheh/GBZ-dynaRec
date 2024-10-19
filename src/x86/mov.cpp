#include <x86Emitter.hpp>


void x86Emitter::xchg8r8r(x86_8 reg1, x86_8 reg2)
{
    emitByte(0x86);
    modRM(mod::RtoR, reg1, reg2);
}

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

void x86Emitter::movabsRBP(uint64_t imm64)
{
    emitByte(0x48);
    emitByte(0xBD);
    emitQWord(imm64);
}

void x86Emitter::mov8rTo8m(x86_8 src)
{
    /*
    * The address of 8m is stored in RBP.
    */
    emitByte(0x88);
    modRM(mod::RtoMdisp8, src, x86_64::RBP);
    emitByte(0);
}

void x86Emitter::mov8mTo8r(x86_8 dest)
{
    /*
    * The address of 8m is stored in RBP.
    */
    emitByte(0x8A);
    modRM(mod::RtoMdisp8, dest, x86_64::RBP);
    emitByte(0);
}


void x86Emitter::mov16mTo16r(x86_16 dest)
{
    emitByte(0x66);
    emitByte(0x8B);
    modRM(mod::RtoMdisp8, dest, x86_64::RBP);
    emitByte(0);
}

void x86Emitter::mov16rTo16m(x86_16 reg)
{
    emitByte(0x66);
    emitByte(0x89);
    modRM(mod::RtoMdisp8, reg, x86_64::RBP);
    emitByte(0);
}
