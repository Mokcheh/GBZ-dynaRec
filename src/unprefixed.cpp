#include <translator.hpp>


using op = Translator;

void op::nop()
{

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

}

void op::add_hl_rp(gbz80::rp src)
{

}

void op::inc_rp(gbz80::rp reg)
{

}

void op::dec_rp(gbz80::rp reg)
{

}

void op::inc_r(gbz80::r reg)
{

}

void op::dec_r(gbz80::r reg)
{

}

void op::ld_r_n(gbz80::r dest)
{

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


void op::add_a(gbz80::r reg)
{

}

void op::adc_a(gbz80::r reg)
{

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

}

void op::adc_a_imm()
{

}

void op::sub_a_imm()
{

}

void op::sbc_a_imm()
{

}

void op::and_a_imm()
{

}

void op::xor_a_imm()
{

}

void op::or_a_imm()
{

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
