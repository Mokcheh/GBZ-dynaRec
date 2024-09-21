#include "cpu.hpp"
#include <dynarec.hpp>
#include <vector>

LR35902::LR35902(): registers({}), alternatives({})
{
}

LR35902::~LR35902()
{

};


int main()
{
    std::vector<uint8_t> testCode = {
        0x3E, 0x07, // LD A, 0x07
        0x47 // LD B, A
    };
    dynaRec dyna(testCode);
    dyna.dispatch(0);
}
