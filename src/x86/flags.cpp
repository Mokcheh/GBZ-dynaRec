#include <x86Emitter.hpp>


void x86Emitter::lahf()
{
    emitByte(0x9F);
}

void x86Emitter::sahf()
{
    emitByte(0x9E);
}

void x86Emitter::stc()
{
    emitByte(0xF9);
}

void x86Emitter::cmc()
{
    emitByte(0xF5);
}


