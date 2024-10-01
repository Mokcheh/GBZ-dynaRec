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
        0x3E, 0, //LD A, 0
        0xCB, 0x37 //SWAP A
    };
    dynaRec dyna(testCode);
    dyna.dispatch(0);
}
