#pragma once
#include <cstdint>
#include <vector>

enum x86_8{
    AL, CL, DL,
    BL, AH, CH,
    DH, BH
};

enum x86_16{
    AX, CX, DX,
    BX, SP, BP,
    SI, DI
};


class x86Emitter{
public:
    x86Emitter(std::vector<uint8_t>& client);
    /*TODO: Implement x86 assembly emitters*/
    void mov(x86_8 Register, uint8_t memory);
    void mov(x86_8 destinationReg, x86_8 sourceReg);
    void mov(x86_16 Register, uint16_t memory);
private:
    void emitByte(uint8_t byte);
    std::vector<uint8_t>& cache;
};


