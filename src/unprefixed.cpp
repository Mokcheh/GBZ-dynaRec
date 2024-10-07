#include "translator.hpp"



void Translator::nop()
{
    cyclesPassed++;
}


void Translator::ld_nnPtr_sp()
{
    const uint16_t n16 = (bus.rom[blockProgramCounter + 1] << 8) | bus.rom[blockProgramCounter + 1];
    const uint64_t memoryAddress = (uint64_t)bus.rom.data() + n16;
    emitter.movabsRBP(memoryAddress);
    emitter.mov16mTo16r(mapR16(gbz80::SP));
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
    const uint16_t imm16 = 
        bus.readMemory(blockProgramCounter) | (bus.readMemory(blockProgramCounter + 1) << 8);
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
    emitter.orStack8r8(1, x86_8::AH);
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
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
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
    emitter.lahf();
    emitter.not8r(mapR8(gbz80::A));
    emitter.arithmetic8r8imm(x86_8::AH, AF, OR);
    emitter.sahf();
    setSubFlag(1);
    cyclesPassed++;
}

void Translator::scf()
{
    emitter.stc();
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, CF, AND);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::ccf()
{
    emitter.cmc();
    emitter.lahf();
    emitter.arithmetic8r8imm(x86_8::AH, CF, AND);
    emitter.sahf();
    setSubFlag(0);
    cyclesPassed++;
}

void Translator::halt()
{

}

void Translator::ld_r_r(gbz80::r dest, gbz80::r src)
{
    emitter.mov8rTo8r(mapR8(dest), mapR8(src));
    cyclesPassed += 2;
}


void Translator::ld_rp_indirect(gbz80::rp ptr, gbz80::r src)
{
    uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64r(x86_64::RBP, x86_64(mapR16(ptr)), ADD);
    emitter.mov8rTo8m(mapR8(src));
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::ldi_hl_indirect(gbz80::r src)
{
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64r(x86_64::RBP, x86_64(mapR16(gbz80::HL)), ADD);
    emitter.mov8rTo8m(mapR8(src));
    emitter.unary16r(mapR16(gbz80::HL), inc);
    emitter.sahf();
    cyclesPassed += 2;
}


void Translator::ldd_hl_indirect(gbz80::r src)
{
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64r(x86_64::RBP, x86_64(mapR16(gbz80::HL)), ADD);
    emitter.mov8rTo8m(mapR8(src));
    emitter.unary16r(mapR16(gbz80::HL), dec);
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::ld_a_rpIndirect(gbz80::rp src)
{
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64r(x86_64::RBP, x86_64(mapR16(src)), ADD);
    emitter.mov8mTo8r(mapR8(gbz80::A));
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::ldi_a_hl_indirect()
{
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64r(x86_64::RBP, x86_64(mapR16(gbz80::HL)), ADD);
    emitter.mov8mTo8r(mapR8(gbz80::A));
    emitter.unary16r(mapR16(gbz80::HL), inc);
    emitter.sahf();
    cyclesPassed += 2;
}

void Translator::ldd_a_hl_indirect()
{
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64r(x86_64::RBP, x86_64(mapR16(gbz80::HL)), ADD);
    emitter.mov8mTo8r(mapR8(gbz80::A));
    emitter.unary16r(mapR16(gbz80::HL), dec);
    emitter.sahf();
    cyclesPassed += 2;
}


void Translator::add_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), ADD);
    cyclesPassed++;
    setSubFlag(0);
}

void Translator::adc_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), ADC);
    cyclesPassed++;
    setSubFlag(0);
}

void Translator::sub_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), SUB);
    cyclesPassed++;
    setSubFlag(1);
}

void Translator::sbc_a(gbz80::r src)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(src), SBB);
    cyclesPassed++;
    setSubFlag(1);
}

void Translator::and_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), AND);
    cyclesPassed++;
    setSubFlag(0);
}

void Translator::xor_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), XOR);
    cyclesPassed++;
    setSubFlag(0);
}

void Translator::or_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), OR);
    cyclesPassed++;
    setSubFlag(0);
}

void Translator::cp_a(gbz80::r reg)
{
    emitter.arithmetic8r8r(mapR8(gbz80::A), mapR8(reg), CMP);
    cyclesPassed++;
    setSubFlag(1);
}

void Translator::ld_indirect_0xffn8_a()
{
    const uint16_t address = 0xFF00 + bus.rom[blockProgramCounter++];
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64imm(x86_64::RBP, address, ADD);
    emitter.mov8rTo8m(mapR8(gbz80::A));
    emitter.sahf();
    cyclesPassed += 3;
}

void Translator::add_sp_e8()
{
    //TODO: this instruction segfaults for some reason
    int8_t e8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic16r16imm(mapR16(gbz80::SP), e8, ADD);
    setSubFlag(0);
    cyclesPassed += 4;
}

void Translator::ld_a_indirect_0xffn8()
{
    const uint16_t address = 0xFF00 + bus.rom[blockProgramCounter++];
    const uint64_t memoryAddress = (uint64_t)bus.rom.data();
    emitter.lahf();
    emitter.movabsRBP(memoryAddress);
    emitter.arithmetic64r64imm(x86_64::RBP, address, ADD);
    emitter.mov8mTo8r(mapR8(gbz80::A));
    emitter.sahf();
    cyclesPassed += 3;
}

void Translator::ld_hl_sp_plus_d()
{
    int8_t e8 = bus.readMemory(blockProgramCounter++);
    emitter.push16r(mapR16(gbz80::SP));
    emitter.arithmetic16r16imm(mapR16(gbz80::SP), e8, ADD);
    emitter.mov16rTo16r(mapR16(gbz80::HL), mapR16(gbz80::SP));
    emitter.pop16r(mapR16(gbz80::SP));
    setSubFlag(0);
    cyclesPassed += 4;
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
    emitter.mov16rTo16r(mapR16(gbz80::SP), mapR16(gbz80::HL));
    cyclesPassed += 2;
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
    const uint16_t n16 = (bus.rom[blockProgramCounter + 1] << 8) | bus.rom[blockProgramCounter + 1];
    const uint64_t memoryAddress = (uint64_t)bus.rom.data() + n16;
    emitter.movabsRBP(memoryAddress);
    emitter.mov8mTo8r(mapR8(gbz80::A));
    blockProgramCounter += 2;
    cyclesPassed += 4;
}

void Translator::pop_rp2(gbz80::rp2 reg)
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
void Translator::push_rp2(gbz80::rp2 reg)
{

}

void Translator::call_nn()
{

}

void Translator::add_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, ADD);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::adc_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, ADC);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::sub_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, SUB);
    setSubFlag(1);
    cyclesPassed += 2;
}

void Translator::sbc_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, SBB);
    setSubFlag(1);
    cyclesPassed += 2;
}

void Translator::and_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, AND);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::xor_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, XOR);
    setSubFlag(0);
    cyclesPassed += 2;

}

void Translator::or_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, OR);
    setSubFlag(0);
    cyclesPassed += 2;
}

void Translator::cp_a_imm()
{
    const uint8_t imm8 = bus.readMemory(blockProgramCounter++);
    emitter.arithmetic8r8imm(mapR8(gbz80::A), imm8, CMP);
    setSubFlag(1);
    cyclesPassed += 2;
}

void Translator::rst(uint8_t vec)
{

}
