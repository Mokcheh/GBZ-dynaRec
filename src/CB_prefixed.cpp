#include <translator.hpp>


void Translator::rlc(gbz80::r target)
{

}

void Translator::rrc(gbz80::r target)
{

}

void Translator::rl(gbz80::r target)
{
    emitter.rotate8r(mapR8(target), 1, rotate::LEFT);
    emitter.sahf();
    emitter.push16r(x86_16::AX);
    emitter.mov8immTo8r(x86_8::AL, 0);
    emitter.arithmetic8r8imm(x86_8::AL, mapR8(target), ADD);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, ZF, AND);
    emitter.arithmetic8stack8r(1, x86_8::AH, OR);
    emitter.pop16r(x86_16::AX);
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::rr(gbz80::r target)
{
    emitter.rotate8r(mapR8(target), 1, rotate::RIGHT);
    emitter.sahf();
    emitter.push16r(x86_16::AX);
    emitter.mov8immTo8r(x86_8::AL, 0);
    emitter.arithmetic8r8imm(x86_8::AL, mapR8(target), ADD);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, ZF, AND);
    emitter.arithmetic8stack8r(1, x86_8::AH, OR);
    emitter.pop16r(x86_16::AX);
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::sla(gbz80::r target)
{
    emitter.shift8r(mapR8(target), 1, shift::LEFT);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, CF + ZF, AND);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::swap(gbz80::r target)
{
    emitter.mov8immTo8r(x86_8::AH, 0);
    emitter.arithmetic8r8r(x86_8::AH, mapR8(target), ADD);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, ZF, AND);
    emitter.push16r(x86_16::AX);
    emitter.mov8rTo8r(x86_8::AH, mapR8(target));
    emitter.shift8r(x86_8::AH, 4, shift::RIGHT);
    emitter.shift8r(mapR8(target), 4, shift::LEFT);
    emitter.arithmetic8r8r(mapR8(target), x86_8::AH, OR);
    if (target == gbz80::A)
        emitter.mov8rTo8stack(0, mapR8(target));
    emitter.pop16r(x86_16::AX);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::srl(gbz80::r target)
{
    emitter.shift8r(mapR8(target), 1, shift::RIGHT);
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
    emitter.shift8r(mapR8(target), 1, shift::RIGHT);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, ZF + CF, AND);
    emitter.arithmetic8r8imm(containerLow, 0b10000000, AND);
    emitter.arithmetic8r8r(mapR8(target), containerLow, OR);
    emitter.pop16r(container);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::bit(uint8_t u3, gbz80::r target)
{
    //TO TEST
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, 0xFE, AND);
    emitter.push16r(x86_16::AX);
    emitter.mov8rTo8r(x86_8::AL, mapR8(target));
    emitter.not8r(x86_8::AL);
    emitter.shift8r(x86_8::AL, u3, shift::RIGHT);
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

