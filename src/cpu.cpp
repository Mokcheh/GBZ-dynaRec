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
        0x3E, 0xBE, //LD A, 0xBE
        0x01, 0x07, 0,
        0x02 //LD (BC), A
    };
    Bus bus(testCode);
    dynaRec dyna(bus);
    dyna.dispatch(0);
}
