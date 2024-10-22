#include <x86Emitter.hpp>

x86Emitter::x86Emitter(std::vector<uint8_t>& client) : 
    cache(client)
{}

void x86Emitter::importFlags()
{

}


void x86Emitter::callFromRbp()
{
    emitByte(0xFF);
    modRM(mod::RtoR, 2, x86_64::RBP);
}

void x86Emitter::__cdeclCallFunction(void *function, x86_16 argument)
{
#ifdef __linux
    movabsRBP((uint64_t)function);
    /*The callee uses the registers, overriding the values
     * So saving them is necessary.
    */
    for(uint8_t reg=0; reg<=x86_16::DI; reg++)
    {
        if(reg != x86_16::SP && reg != x86_16::BP)
            push16r(x86_16(reg));
    }
    mov16rTo16r(x86_16::DI, argument);
    callFromRbp();
    for(uint8_t reg=x86_16::DI; reg > x86_16::AX; reg--)
    {
        if(reg != x86_16::SP && reg != x86_16::BP)
            pop16r(x86_16(reg));
    }
#elif __WIN32
#endif

}

void x86Emitter::emitByte(uint8_t byte)
{
    cache.emplace_back(byte);
}

void x86Emitter::emitWord(uint16_t word)
{
    cache.emplace_back(word & 0xFF);
    cache.emplace_back(word >> 8);
}

void x86Emitter::modRM(uint8_t mod, uint8_t reg, uint8_t rm)
{
    const uint8_t result = (mod << 6) | (reg << 3) | (rm & 0b111);
    emitByte(result);
}

void x86Emitter::sib(uint8_t scale, uint8_t index, uint8_t base)
{
    const uint8_t result = (scale << 6) | (index << 3) | (base & 0b111);
    emitByte(result);
}

void x86Emitter::emitQWord(uint64_t qword)
{
    for (int i = 0; i < 8; i++) {
        emitByte(qword & 0xFF);
        qword >>= 8;
    }
}
