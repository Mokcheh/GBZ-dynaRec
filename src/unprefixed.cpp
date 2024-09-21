#include <translator.hpp>


using op = Translator;

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


void op::nop()
{
    cyclesPassed++;
}


void op::ld_nnPtr_sp()
{

}

void op::stop()
{

}

void op::jr(gbz80::r relative)
{

}

void op::jr_cc(gbz80::r relative, uint8_t cc)
{

}

void op::ld_rp_nn(gbz80::rp dest)
{
    const uint16_t imm16 = source[blockProgramCounter] | (source[blockProgramCounter + 1] << 8);
    emitter.mov16immTo16r(mapR16(dest), imm16);
    blockProgramCounter += 2;
    cyclesPassed += 3;
}

void op::add_hl_rp(gbz80::rp src)
{
    /*
     * This instruction sets the half carry flag when overflow is generated from bit 11.
     * The problem with x86_64 is that the half carry flag is set only if carry/borrow is generated from
     * the lowest 4 bits. And there is no instruction to set the flag manually.
    */
    emitter.lahf();
    emitter.push16r(x86_16::AX);
    emitter.mov16rTo16r(x86_16::AX, mapR16(src));
    emitter.add8r8r(mapR8(gbz80::L), x86_8::AL);
    emitter.adc8r8r(mapR8(gbz80::H), x86_8::AH);
    emitter.lahf();
    emitter.andr8imm8(x86_8::AH, 0b10001);
    emitter.orStack8r8(0, x86_8::AH);
    emitter.pop16r(x86_16::AX);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed += 2;
}

void op::inc_rp(gbz80::rp reg)
{
    emitter.unary16r(mapR16(reg), inc);
    cyclesPassed += 2;
}

void op::dec_rp(gbz80::rp reg)
{
    emitter.unary16r(mapR16(reg), dec);
    cyclesPassed += 2;
}

void op::inc_r(gbz80::r reg)
{
    emitter.unary8r(mapR8(reg), inc);
    cyclesPassed++;
}

void op::dec_r(gbz80::r reg)
{
    emitter.unary8r(mapR8(reg), dec);
    cyclesPassed++;
}

void op::ld_r_n(gbz80::r dest)
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.mov8immTo8r(mapR8(dest), imm8);
    cyclesPassed += 2;
}

void op::rlca()
{

}

void op::rrca()
{

}

void op::rla()
{

}

void op::rra()
{

}

void op::dda()
{

}

void op::cpl()
{

}

void op::scf()
{

}

void op::ccf()
{

}

void op::halt()
{

}

void op::ld_r_r(gbz80::r dest, gbz80::r src)
{
    emitter.mov8rTo8r(mapR8(dest), mapR8(src));
    cyclesPassed += 2;
}


void op::ld_rp_indirect(gbz80::rp dest, gbz80::r src)
{

}

void op::ldi_hl_indirect(gbz80::r src)
{

}


void op::ldd_hl_indirect(gbz80::r src)
{

}

void op::ld_a_rpIndirect(gbz80::rp src)
{

}

void op::ldi_a_hl_indirect()
{

}

void op::ldd_a_hl_indirect()
{

}


void op::add_a(gbz80::r src)
{
    emitter.add8r8r(mapR8(gbz80::A), mapR8(src));
    cyclesPassed++;
    setSubFlag(0);
}

void op::adc_a(gbz80::r src)
{
    emitter.adc8r8r(mapR8(gbz80::A), mapR8(src));
    cyclesPassed++;
    setSubFlag(0);
}

void op::sub_a(gbz80::r reg)
{

}

void op::sbc_a(gbz80::r reg)
{

}

void op::and_a(gbz80::r reg)
{

}

void op::xor_a(gbz80::r reg)
{

}

void op::or_a(gbz80::r reg)
{

}

void op::cp_a(gbz80::r reg){


}

void op::ld_indirect_0xff00Plusn8_a()
{

}

void op::add_sp_immediate()
{

}

void op::ld_a_indirect_0xff00Plusn8()
{

}

void op::ld_hl_sp_plus_d()
{

}

void op::ret_cc(uint8_t cc)
{

}

void op::ret()
{

}

void op::reti()
{

}

void op::jp_hl()
{

}

void op::ld_sp_hl()
{

}

void op::jp_cc_nn(uint8_t cc)
{

}


void op::ld_indirect_nnPlusC_a()
{

}

void op::ld_indirect_nn_a()
{

}

void op::ld_a_indirect_0xff00PlusC()
{

}

void op::ld_a_indirect_nn()
{

}

void op::pop_rp2(gbz80::rp2::reg reg)
{

}


void op::jp_nn()
{

}

void op::di()
{

}

void op::ei()
{

}

void op::call_cc_nn(uint8_t cc)
{

}
void op::push_rp2(gbz80::rp2::reg reg)
{

}

void op::call_nn()
{

}

void op::add_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, Add);
    setSubFlag(0);
    cyclesPassed += 2;
}

void op::adc_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, Adc);
    setSubFlag(0);
    cyclesPassed += 2;
}

void op::sub_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, Sub);
    setSubFlag(1);
    cyclesPassed += 2;
}

void op::sbc_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, Sbb);
    setSubFlag(1);
    cyclesPassed += 2;
}

void op::and_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, And);
    setSubFlag(0);
    cyclesPassed += 2;
}

void op::xor_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, Xor);
    setSubFlag(0);
    cyclesPassed += 2;

}

void op::or_a_imm()
{
    const uint8_t imm8 = source[blockProgramCounter++];
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, Or);
    setSubFlag(0);
    cyclesPassed += 2;
}

void op::cp_a_imm()
{

}

void op::rst(uint8_t vec)
{

}

void op::bit(uint8_t u3, gbz80::r target)
{

}

void op::res(uint8_t u3, gbz80::r target)
{

}

void op::set(uint8_t u3, gbz80::r target)
{

}



void op::rlc(gbz80::r target)
{

}

void op::rrc(gbz80::r target)
{

}

void op::rl(gbz80::r target)
{

}

void op::rr(gbz80::r target)
{

}

void op::sla(gbz80::r target)
{

}

void op::swap(gbz80::r target)
{

}

void op::srl(gbz80::r target)
{

}

void op::sra(gbz80::r target)
{

}
