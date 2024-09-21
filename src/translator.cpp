#include <translator.hpp>

Translator::Translator(std::vector<uint8_t>& clientCache, std::vector<uint8_t>& targetCode):
    output(clientCache), emitter(clientCache), source(targetCode), cyclesPassed(0), jmpOccured(false)
{

}

void Translator::translateBlock()
{
    do{
        const uint8_t opcode = source[blockProgramCounter++];
        if(opcode == 0xCB)
            decodeAndRunCB(opcode);
        else
            decodeAndRun(opcode);
    }while(blockProgramCounter < source.size()/**!jmpOccured**/);
}

void Translator::setSubFlag(bool flag)
{
    emitter.mov16immTo16r(x86_16(DI), flag);
}

void Translator::decodeAndRun(uint8_t opcode)
{
    /*
     * Source:
     * https://gb-archive.github.io/salvage/decoding_gbz80_opcodes/Decoding Gamboy Z80 Opcodes.html
    */
    const uint8_t x = opcode >> 6;
    const uint8_t y = (opcode >> 3) & 0b111;
    const uint8_t z = opcode & 0b111;
    const uint8_t p = y >> 1;
    const uint8_t q = y & 1;
    switch(x){
    case 0:
    {
        switch (z) {
        case 0: switch(y){
            case 0: nop();break;
            case 1: ld_nnPtr_sp();break;
            case 2: stop();break;
            case 3: jr(gbz80::D);break;
            default: jr_cc(gbz80::D, y-4);
            }break;

        case 1: switch(q){
            case 0: ld_rp_nn(gbz80::rp(p));break;
            case 1: add_hl_rp(gbz80::rp(p));break;
            }break;

        case 2:
        {
            if(q == 0){
                switch(p){
                case 0: ld_rp_indirect(gbz80::BC, gbz80::A);break;
                case 1: ld_rp_indirect(gbz80::DE, gbz80::A);break;
                case 2: ldi_hl_indirect(gbz80::A);break;
                case 3: ldd_hl_indirect(gbz80::A);break;
                }
            }else{
                switch(p){
                case 0: ld_a_rpIndirect(gbz80::BC);break;
                case 1: ld_a_rpIndirect(gbz80::DE);break;
                case 2: ldi_a_hl_indirect();break;
                case 3: ldd_a_hl_indirect();break;
                }
            }
        }break;

        case 3: switch(q){
            case 0: inc_rp(gbz80::rp(p));break;
            case 1: dec_rp(gbz80::rp(p));break;
            }break;

        case 4: inc_r(gbz80::r(y));break;
        case 5: dec_r(gbz80::r(y));break;
        case 6: ld_r_n(gbz80::r(y));break;

        case 7: switch(y){
            case 0: rlca();break;
            case 1: rrca();break;
            case 2: rla();break;
            case 3: rra();break;
            case 4: dda();break;
            case 5: cpl();break;
            case 6: scf();break;
            case 7: ccf();break;
            }break;
        }
    }
    break;
    case 1:
    {
        if(z == 6 && y == 6)
            halt();
        else
            ld_r_r(gbz80::r(y), gbz80::r(z));
    }break;
    case 2: switch(y){
        case 0: add_a(gbz80::r(z)); break;
        case 1: adc_a(gbz80::r(z)); break;
        case 2: sub_a(gbz80::r(z)); break;
        case 3: sbc_a(gbz80::r(z)); break;
        case 4: and_a(gbz80::r(z)); break;
        case 5: xor_a(gbz80::r(z)); break;
        case 6: or_a(gbz80::r(z)); break;
        case 7: cp_a(gbz80::r(z)); break;
        }break;

    case 3:
        switch(z){
        case 0: switch(y){
            case 0 ... 3:
                ret_cc(y); break;
            case 4: ld_indirect_0xff00Plusn8_a(); break;
            case 5: add_sp_immediate(); break;
            case 6: ld_a_indirect_0xff00Plusn8(); break;
            case 7: ld_hl_sp_plus_d(); break;
            }break;
        case 1:
        {
            if(q ==0)
                pop_rp2(gbz80::rp2::reg(p));
            else{
                switch(p){
                case 0: ret(); break;
                case 1: reti(); break;
                case 2: jp_hl(); break;
                case 3: ld_sp_hl(); break;
                }
            }
        }break;
        case 2: switch(y){
            case 0 ... 3:
                jp_cc_nn(y); break;
            case 4: ld_indirect_nnPlusC_a(); break;
            case 5: ld_indirect_nn_a(); break;
            case 6: ld_a_indirect_0xff00PlusC(); break;
            case 7: ld_a_indirect_nn(); break;
            }break;

        case 3: switch(y){
            case 0: jp_nn(); break;
            case 6: di(); break;
            case 7: ei(); break;
            }break;

        case 4:
        {
            if(y <= 3)
                call_cc_nn(y);
        }break;

        case 5:
        {
            if(q == 0)
                push_rp2(gbz80::rp2::reg(p));
            else if(q == 1 && p == 0)
                call_nn();
        }break;

        case 6: switch(y){
            case 0: add_a_imm(); break;
            case 1: adc_a_imm(); break;
            case 2: sub_a_imm(); break;
            case 3: sbc_a_imm(); break;
            case 4: and_a_imm(); break;
            case 5: xor_a_imm(); break;
            case 6: or_a_imm(); break;
            case 7: cp_a_imm(); break;
            }break;
        case 7: rst(y*8); break;
        default: break;
        }
    }

}

void Translator::decodeAndRunCB(uint8_t opcode)
{
    const uint8_t x = opcode >> 6;
    const uint8_t y = (opcode >> 3) & 0b111;
    const uint8_t z = opcode & 0b111;

    switch(x){
    case 0: switch(y){
        case 0: rlc(gbz80::r(z)); break;
        case 1: rrc(gbz80::r(z)); break;
        case 2: rl(gbz80::r(z)); break;
        case 3: rr(gbz80::r(z)); break;
        case 4: sla(gbz80::r(z)); break;
        case 5: sra(gbz80::r(z)); break;
        case 6: swap(gbz80::r(z)); break;
        case 7: srl(gbz80::r(z)); break;
        }break;

    case 1: bit(y, gbz80::r(z)); break;
    case 2: res(y, gbz80::r(z)); break;
    case 3: set(y, gbz80::r(z)); break;
    }

}