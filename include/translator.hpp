#pragma once

#include <x86_64Emitter.hpp>
#include <bus.hpp>
#include <memory>

namespace gbz80{
    enum r : uint8_t{
        B = 0, C = 1, D = 2, E = 3,
        H = 4, L = 5, HLptr = 6, A = 7
    };

    enum rp : uint8_t{
        BC = 0, DE = 1, HL = 2, SP = 3,
    };

    enum class rp2 : uint8_t {
        BC = 0, DE = 1, HL = 2, AF = 3
    };
}

class Translator{
public:
    Translator(std::vector<uint8_t>& clientCache, uint16_t start, Bus& bus);
    void translateBlock();
    uint16_t blockProgramCounter;
    uint32_t cyclesPassed;
    uint16_t getJumpAddress();
    std::shared_ptr<uint16_t> getReturnAddress();
    static x86_8 mapR8(gbz80::r r);
    static x86_16 mapR16(gbz80::rp rp);
private:
    std::shared_ptr<uint16_t> returnAddress;
    bool stopHit;
    x64Emitter x64;
    Bus& bus;
    std::vector<uint8_t>& output;
    uint16_t jumpAddress;
    void setSubFlag(bool flag);
    void generateGBZ80FlagsFromX64();
    /*TODO: Implement GBZ instruction in x86 assembly.*/
    /* Source:
     * https://izik1.github.io/gbops/
    */
    void importGBZ80FlagsToX64();
    void decodeAndRun(uint8_t opcode);
    void decodeAndRunCB(uint8_t opcode);
    void nop();
    void ld_nnPtr_sp();
    void stop();
    void jr(gbz80::r relative);
    void jr_cc(gbz80::r relative, uint8_t cc);
    void ld_rp_nn(gbz80::rp dest);
    void add_hl_rp(gbz80::rp src);
    void inc_rp(gbz80::rp reg);
    void dec_rp(gbz80::rp reg);
    void inc_r(gbz80::r reg);
    void dec_r(gbz80::r reg);
    void ld_r_n(gbz80::r dest);
    void rlca(); void rrca(); void rla();
    void rra(); void daa(); void cpl();
    void scf(); void ccf(); void halt();
    void ld_r_r(gbz80::r dest, gbz80::r src);
    void ld_rp_indirect(gbz80::rp dest, gbz80::r src);
    void ldi_hl_indirect(gbz80::r src);
    void ldd_hl_indirect(gbz80::r src);
    void ld_a_rpIndirect(gbz80::rp src);
    void ldi_a_hl_indirect();
    void ldd_a_hl_indirect();
    void add_a(gbz80::r reg);
    void adc_a(gbz80::r reg);
    void sub_a(gbz80::r reg);
    void sbc_a(gbz80::r reg);
    void and_a(gbz80::r reg);
    void xor_a(gbz80::r reg);
    void or_a(gbz80::r reg);
    void cp_a(gbz80::r reg);
    void ret_cc(uint8_t cc);
    void ld_indirect_0xffn8_a();
    void add_sp_e8();
    void ld_a_indirect_0xffn8();
    void ld_hl_sp_plus_d();
    void pop_rp2(gbz80::rp2 reg);
    void ret();
    void reti();
    void jp_hl();
    void ld_sp_hl();
    void jp_cc_nn(uint8_t cc);
    void ldh_c_a();
    void ld_indirect_nn_a();
    void ldh_a_c();
    void ld_a_indirect_nn();
    void jp_nn();
    void di();
    void ei();
    void call_cc_nn(uint8_t cc);
    void push_rp2(gbz80::rp2 reg);
    void call_nn();
    void add_a_imm();
    void adc_a_imm();
    void sub_a_imm();
    void sbc_a_imm();
    void and_a_imm();
    void xor_a_imm();
    void or_a_imm();
    void cp_a_imm();
    void rst(uint8_t vec);
    void bit(uint8_t u3, gbz80::r target);
    void res(uint8_t u3, gbz80::r target);
    void set(uint8_t u3, gbz80::r target);
    void rlc(gbz80::r target);
    void rrc(gbz80::r target);
    void rl(gbz80::r target);
    void rr(gbz80::r target);
    void sla(gbz80::r target);
    void swap(gbz80::r target);
    void srl(gbz80::r target);
    void sra(gbz80::r target);
};
