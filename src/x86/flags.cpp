#include <x86Emitter.hpp>


void x86Emitter::lahf()
{
    emitByte(0x9F);
}

void x86Emitter::sahf()
{
    emitByte(0x9E);
}

