#include <translator.hpp>

void emulateZF(x64Emitter& emitter, const gbz80::r target)
{
    x86_16 temp;
    x86_8 tempLow;
    if(target == gbz80::A)
    {
        temp = x86_16::BX;
        tempLow = x86_8::BL;
    }
    else
    {
        temp = x86_16::AX;
        tempLow = x86_8::AL;
    }
    emitter.push16r(temp);
    emitter.lahf();
    emitter.arithmetic8r8imm(tempLow, 0, ADD);
    emitter.mov8rTo8r(tempLow, x86_8::AH);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, ZF, AND);
    emitter.arithmetic8r8r(tempLow, x86_8::AH, OR);
    emitter.mov8rTo8r(x86_8::AH, tempLow);
    emitter.sahf();
    emitter.pop16r(temp);
}

void Translator::rlc(gbz80::r target)
{
    x64.bitwise8r(mapR8(target), (uint8_t)rotate::LEFT);
    emulateZF(x64, target);
    setSubFlag(0);
}

void Translator::rrc(gbz80::r target)
{
    x64.bitwise8r(mapR8(target), (uint8_t)rotate::RIGHT);
    emulateZF(x64, target);
    setSubFlag(0);
}

void Translator::rl(gbz80::r target)
{
    x64.bitwise8r(mapR8(target), (uint8_t)RC::LEFT);
    emulateZF(x64, target);
    setSubFlag(0);
}

void Translator::rr(gbz80::r target)
{
    x64.bitwise8r(mapR8(target), (uint8_t)RC::RIGHT);
    emulateZF(x64, target);
    setSubFlag(0);
}

void Translator::sla(gbz80::r target)
{
    x64.bitwise8r(mapR8(target), 1, (uint8_t)shift::LEFT);
    x64.lahf();
    x64.arithmetic8r8imm(x86_8::AH, CF + ZF, AND);
    x64.sahf();
    setSubFlag(0);
}

void Translator::swap(gbz80::r target)
{
    x64.mov8immTo8r(x86_8::AH, 0);
    x64.arithmetic8r8r(x86_8::AH, mapR8(target), ADD);
    x64.lahf();
    x64.arithmetic8r8imm(x86_8::AH, ZF, AND);
    x64.push16r(x86_16::AX);
    x64.mov8rTo8r(x86_8::AH, mapR8(target));
    x64.bitwise8r(x86_8::AH, 4, (uint8_t)shift::RIGHT);
    x64.bitwise8r(mapR8(target), 4, (uint8_t)shift::LEFT);
    x64.arithmetic8r8r(mapR8(target), x86_8::AH, OR);
    if (target == gbz80::A)
        x64.mov8rTo8stack(0, mapR8(target));
    x64.pop16r(x86_16::AX);
    x64.sahf();
    setSubFlag(0);
}

void Translator::srl(gbz80::r target)
{
    x64.bitwise8r(mapR8(target), 1, (uint8_t)shift::RIGHT);
    x64.lahf();
    x64.arithmetic8r8imm(x86_8::AH, CF + ZF, AND);
    x64.sahf();
    setSubFlag(0);
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
    x64.push16r(container);
    x64.mov8rTo8r(containerLow, mapR8(target));
    x64.bitwise8r(mapR8(target), 1, (uint8_t)shift::RIGHT);
    x64.lahf();
    x64.arithmetic8r8imm(x86_8::AH, ZF + CF, AND);
    x64.arithmetic8r8imm(containerLow, 0b10000000, AND);
    x64.arithmetic8r8r(mapR8(target), containerLow, OR);
    x64.pop16r(container);
    x64.sahf();
    setSubFlag(0);
}

void Translator::bit(uint8_t u3, gbz80::r target)
{
    //TO TEST
    x64.lahf();
    x64.arithmetic8r8imm(x86_8::AH, 0xFE, AND);
    x64.push16r(x86_16::AX);
    x64.mov8rTo8r(x86_8::AL, mapR8(target));
    x64.not8r(x86_8::AL);
    x64.bitwise8r(x86_8::AL, u3, (uint8_t)shift::RIGHT);
    x64.arithmetic8r8imm(x86_8::AL, 1, AND);
    x64.arithmetic8r8r(x86_8::AH, x86_8::AL, OR);
    x64.sahf();
    x64.pop16r(x86_16::AX);
    setSubFlag(0);
}

void Translator::res(uint8_t u3, gbz80::r target)
{
    const uint8_t bit = ~(1 << u3);
    x64.lahf();
    x64.arithmetic8r8imm(mapR8(target), bit, AND);
    x64.sahf();
}

void Translator::set(uint8_t u3, gbz80::r target)
{
    const uint8_t bit = 1 << u3;
    x64.lahf();
    x64.arithmetic8r8imm(mapR8(target), bit, OR);
    x64.sahf();
}

