#pragma once
#include <cstdint>
#include <vector>

enum x86_8: uint8_t{
    AL, CL, DL,
    BL, AH, CH,
    DH, BH
};

enum x86_16: uint8_t{
    AX, CX, DX,
    BX, SP, BP,
    SI, DI,
};

enum x86_64: uint8_t{
    RAX, RCX, RDX,
    RBX, RSP, RBP,
    RSI, RDI,
    SIB = 0b100
};

enum mod: uint8_t{RtoM = 0b00, RtoMdisp8 = 0b01, RtoMdisp32 = 0b10, RtoR = 0b11};

enum unary: bool{inc = 0, dec = 1};
enum arithmetic: uint8_t{Add = 0, Adc = 2, Sub = 5, Sbb = 3, And = 4, Xor = 6,  Or = 1};

class x86Emitter{
public:
    x86Emitter(std::vector<uint8_t>& client);
    /*TODO: Implement x86 assembly emitters*/
    void mov8immTo8r(x86_8 r8, uint8_t imm8);
    void mov16immTo16r(x86_16 r16, uint16_t imm16);
    void mov16rTo16r(x86_16 dest, x86_16 src);
    void mov8rTo8r(x86_8 dest, x86_8 src);
    void unary8r(x86_8 r8, unary op);
    void unary16r(x86_16 r16, unary op);
    void arithmetic8r8imm(x86_8 dest, uint8_t imm8, arithmetic op);
    void adc8r8r(x86_8 dest, x86_8 src);
    void add16r16r(x86_16 dest, x86_16 src);
    void add8r8r(x86_8 dest, x86_8 src);
    void push16r(x86_16 reg);
    void pop16r(x86_16 reg);
    void lahf();
    void sahf();
    void andr8imm8(x86_8 target, uint8_t imm8);
    void orStack8r8(uint8_t offset, x86_8 src);
    //void inc8m(x86_16 r16);
private:
    void emitByte(uint8_t byte);
    void emitWord(uint16_t word);
    void modRM(uint8_t mod, uint8_t reg, uint8_t rm);
    void sib(uint8_t scale, uint8_t index, uint8_t base);
    std::vector<uint8_t>& cache;
};


