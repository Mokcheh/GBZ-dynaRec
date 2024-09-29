#include <translator.hpp>


void Translator::rlc(gbz80::r target)
{

}

void Translator::rrc(gbz80::r target)
{

}

void Translator::rl(gbz80::r target)
{

}

void Translator::rr(gbz80::r target)
{

}

void Translator::sla(gbz80::r target)
{
    emitter.shift8r(mapR8(target), 1, LEFT);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, CF + ZF, AND);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::swap(gbz80::r target)
{

}

void Translator::srl(gbz80::r target)
{
    emitter.shift8r(mapR8(target), 1, RIGHT);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, CF + ZF, AND);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::sra(gbz80::r target)
{
    x86_16 container;
    x86_8 containerLow;
    if (target == gbz80::A)
    {
        container = x86_16::BX;
        containerLow = x86_8::BL;
    }
    else
    {
        container = x86_16::AX;
        containerLow = x86_8::AL;
    }
    emitter.push16r(container);
    emitter.mov8rTo8r(containerLow, mapR8(target));
    emitter.shift8r(mapR8(target), 1, RIGHT);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, ZF + CF, AND);
    emitter.arithmetic8r8imm(containerLow, 0b10000000, AND);
    emitter.arithmetic8r8r(mapR8(target), containerLow, OR);
    emitter.pop16r(container);
    emitter.sahf();
    setSubFlag(0);
}

void Translator::bit(uint8_t u3, gbz80::r target)
{
    //TO TEST
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, 0xFE, AND);
    emitter.push16r(x86_16::AX);
    emitter.mov8rTo8r(x86_8::AL, mapR8(target));
    emitter.not8r(x86_8::AL);
    emitter.shift8r(x86_8::AL, u3, RIGHT);
    emitter.arithmetic8r8imm(x86_8::AL, 1, AND);
    emitter.arithmetic8r8r(x86_8::AH, x86_8::AL, OR);
    emitter.sahf();
    emitter.pop16r(x86_16::AX);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::res(uint8_t u3, gbz80::r target)
{
    const uint8_t bit = ~(1 << u3);
    emitter.lahf();
    emitter.arithmetic8r8imm(mapR8(target), bit, AND);
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::set(uint8_t u3, gbz80::r target)
{
    const uint8_t bit = 1 << u3;
    emitter.lahf();
    emitter.arithmetic8r8imm(mapR8(target), bit, OR);
    emitter.sahf();
    cyclesPassed += 2;
}

