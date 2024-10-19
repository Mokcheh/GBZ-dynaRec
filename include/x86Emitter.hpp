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

enum mod: uint8_t{ RtoM = 0b00, RtoMdisp8 = 0b01, RtoMdisp32 = 0b10, RtoR = 0b11 };
enum unary: bool{ inc = 0, dec = 1 };
enum arithmetic: uint8_t{ ADD, OR, ADC, SBB, AND, SUB, XOR, CMP };

enum class shift : uint8_t { LEFT = 4, RIGHT = 5 };
enum class rotate : uint8_t { LEFT = 0, RIGHT = 1 };
enum class RC: uint8_t { LEFT = 2, RIGHT = 3 };

class x86Emitter{
public:
    x86Emitter(std::vector<uint8_t>& client);
    void importFlags();
    void _cdeclCallFunction(void* function, x86_16 argument);
    /*TODO: Implement x86 assembly emitters*/
    void xchg8r8r(x86_8 reg1, x86_8 reg2);
    void mov8immTo8r(x86_8 r8, uint8_t imm8);
    void mov16immTo16r(x86_16 r16, uint16_t imm16);
    void mov16rTo16r(x86_16 dest, x86_16 src);
    void mov8rTo8r(x86_8 dest, x86_8 src);
    void mov8rTo8m(x86_8 src);
    void mov16mTo16r(x86_16 dest);
    void mov16rTo16m(x86_16 reg);
    void mov8mTo8r(x86_8 dest);
    void movabsRBP(uint64_t imm64);
    void unary8r(x86_8 r8, unary op); 
    void unary16r(x86_16 r16, unary op);
    void arithmetic8r8imm(x86_8 dest, uint8_t imm8, arithmetic op);
    void arithmetic8r8r(x86_8 dest, x86_8 src, arithmetic op);
    void arithmetic16r16imm(x86_16 dest, uint16_t imm16, arithmetic op);
    void arithmetic64r64r(x86_64 dest, x86_64 src, arithmetic op);
    void arithmetic64r64imm(x86_64 dest, uint64_t imm64, arithmetic op);
    void add16r16r(x86_16 dest, x86_16 src);
    void push16r(x86_16 reg); void pop16r(x86_16 reg);
    void lahf(); void sahf();
    void arithmetic8stack8r(uint8_t offset, x86_8 src, arithmetic op);
    void arithmetic8stack8imm(uint8_t offset, uint8_t imm8, arithmetic op);
    void mov8rTo8stack(uint8_t offset, x86_8 src);
    void cmc(); void stc();
    void not8r(x86_8 reg);
    void shift8r(x86_8 target, uint8_t units, shift direction);
    void rotate8r(x86_8 target, uint8_t offset, rotate op);
    void rotateWithCarry(x86_8 target, RC direction);
private:
    void emitByte(uint8_t byte);
    void emitWord(uint16_t word);
    void emitQWord(uint64_t qword);
    void modRM(uint8_t mod, uint8_t reg, uint8_t rm);
    void sib(uint8_t scale, uint8_t index, uint8_t base);
    void push64r(x86_64 reg);
    void callFromRbp();
    std::vector<uint8_t>& cache;
};


enum eflags{CF = 0b1, AF = 0b10000, ZF = 0b1000000};
