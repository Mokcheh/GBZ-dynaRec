#include <x86_64Emitter.hpp>


void x64Emitter::lahf()
{
    emitByte(0x9F);
}

void x64Emitter::sahf()
{
    emitByte(0x9E);
}

void x64Emitter::stc()
{
    emitByte(0xF9);
}

void x64Emitter::cmc()
{
    emitByte(0xF5);
}


