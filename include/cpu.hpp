#pragma once

#include <cstdint>


struct Registers{
    uint8_t
        A,
        F,
        B,
        C,
        D,
        E,
        H,
        L;
};

class LR35902{
public:
    LR35902();
    ~LR35902();
private:
    Registers registers;
    Registers alternatives;
    uint16_t
        stackPtr,
        programCounter;
};
