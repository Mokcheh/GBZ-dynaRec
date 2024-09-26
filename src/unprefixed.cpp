#include "translator.hpp"


x86_8 mapR8(gbz80::r r)
{
    x86_8 reg;
    using namespace gbz80;
    switch(r){
    case A: reg = x86_8::AL; break;
    case B: reg = x86_8::CH; break;
    case C: reg = x86_8::CL; break;
    case D: reg = x86_8::BH; break;
    case E: reg = x86_8::BL; break;
    case H: reg = x86_8::DH; break;
    case L: reg = x86_8::DL; break;
    default: reg = x86_8::AH; break;
    }
    return reg;
}
x86_16 mapR16(gbz80::rp rp)
{
    x86_16 reg;
    using namespace gbz80;
    switch(rp){
    case rp::BC: reg = x86_16::CX; break;
    case rp::DE: reg = x86_16::BX; break;
    case rp::HL: reg = x86_16::DX; break;
    case rp::SP: reg = x86_16::SI; break;
    }
    return reg;
}


void Translator::nop()
{
    cyclesPassed++;
}


void Translator::ld_nnPtr_sp()
{

}

void Translator::stop()
{

}

void Translator::jr(gbz80::r relative)
{

}

void Translator::jr_cc(gbz80::r relative, uint8_t cc)
{

}

void Translator::ld_rp_nn(gbz80::rp dest)
{
    const uint16_t imm16 = source[blockProgramCounter] | (source[blockProgramCounter + 1] << 8);
    emitter.mov16immTo16r(mapR16(dest), imm16);
    blockProgramCounter += 2;
    cyclesPassed += 3;
}

void Translator::add_hl_rp(gbz80::rp src)
{
    /*
     * This instruction sets the half carry flag when overflow is generated from bit 11.
     * The problem with x86_64 is that the half carry flag is set only if carry/borrow is generated from
     * the lowest 4 bits. And there is no instruction to set the flag manually.
    */
    emitter.lahf();
    emitter.push16r(x86_16::AX);
    emitter.mov16rTo16r(x86_16::AX, mapR16(src));
    emitter.arithmetic8r8r(mapR8(gbz80::L), x86_8::AL, ADD);
    emitter.arithmetic8r8r(mapR8(gbz80::H), x86_8::AH, ADC);
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, 0b10001, AND);
    emitter.orStack8r8(0, x86_8::AH);
    emitter.pop16r(x86_16::AX);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::inc_rp(gbz80::rp reg)
{
    emitter.unary16r(mapR16(reg), inc);
    cyclesPassed += 2;
}

void Translator::dec_rp(gbz80::rp reg)
{
    emitter.unary16r(mapR16(reg), dec);
    cyclesPassed += 2;
}

void Translator::inc_r(gbz80::r reg)
{
    emitter.unary8r(mapR8(reg), inc);
    cyclesPassed++;
}

void Translator::dec_r(gbz80::r reg)
{
    emitter.unary8r(mapR8(reg), dec);
    cyclesPassed++;
}

void Translator::ld_r_n(gbz80::r dest)
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.mov8immTo8r(mapR8(dest), imm8);
    cyclesPassed += 2;
}

void Translator::rlca()
{

}

void Translator::rrca()
{

}

void Translator::rla()
{

}

void Translator::rra()
{

}

void Translator::dda()
{

}

void Translator::cpl()
{

}

void Translator::scf()
{

}

void Translator::ccf()
{

}

void Translator::halt()
{

}

void Translator::ld_r_r(gbz80::r dest, gbz80::r src)
{
    emitter.mov8rTo8r(mapR8(dest), mapR8(src));
    cyclesPassed += 2;
}


void Translator::ld_rp_indirect(gbz80::rp dest, gbz80::r src)
{

}

void Translator::ldi_hl_indirect(gbz80::r src)
{

}


void Translator::ldd_hl_indirect(gbz80::r src)
{

}

void Translator::ld_a_rpIndirect(gbz80::rp src)
{

}

void Translator::ldi_a_hl_indirect()
{

}

void Translator::ldd_a_hl_indirect()
{

}


void Translator::add_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), ADD);
    cyclesPassed++;
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::adc_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), ADC);
    cyclesPassed++;
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::sub_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), SUB);
    cyclesPassed++;
    setSubFlag(1);
    cyclesPassed++;
}

void Translator::sbc_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), SBB);
    cyclesPassed++;
    setSubFlag(1);
    cyclesPassed++;
}

void Translator::and_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), AND);
    cyclesPassed++;
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::xor_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), XOR);
    cyclesPassed++;
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::or_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), OR);
    cyclesPassed++;
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::cp_a(gbz80::r reg)
{

}

void Translator::ld_indirect_0xff00Plusn8_a()
{

}

void Translator::add_sp_immediate()
{

}

void Translator::ld_a_indirect_0xff00Plusn8()
{

}

void Translator::ld_hl_sp_plus_d()
{

}

void Translator::ret_cc(uint8_t cc)
{

}

void Translator::ret()
{

}

void Translator::reti()
{

}

void Translator::jp_hl()
{

}

void Translator::ld_sp_hl()
{

}

void Translator::jp_cc_nn(uint8_t cc)
{

}


void Translator::ld_indirect_nnPlusC_a()
{

}

void Translator::ld_indirect_nn_a()
{

}

void Translator::ld_a_indirect_0xff00PlusC()
{

}

void Translator::ld_a_indirect_nn()
{

}

void Translator::pop_rp2(gbz80::rp2::reg reg)
{

}


void Translator::jp_nn()
{

}

void Translator::di()
{

}

void Translator::ei()
{

}

void Translator::call_cc_nn(uint8_t cc)
{

}
void Translator::push_rp2(gbz80::rp2::reg reg)
{

}

void Translator::call_nn()
{

}

void Translator::add_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, ADD);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::adc_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, ADC);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::sub_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, SUB);
    setSubFlag(1);
    cyclesPassed += 2;
}

void Translator::sbc_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, SBB);
    setSubFlag(1);
    cyclesPassed += 2;
}

void Translator::and_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, AND);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::xor_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, XOR);
    setSubFlag(0);
    cyclesPassed += 2;

}

void Translator::or_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, OR);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::cp_a_imm()
{

}

void Translator::rst(uint8_t vec)
{

}

void Translator::bit(uint8_t u3, gbz80::r target)
{

}

void Translator::res(uint8_t u3, gbz80::r target)
{

}

void Translator::set(uint8_t u3, gbz80::r target)
{

}

