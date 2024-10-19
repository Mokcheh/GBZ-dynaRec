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
        0x3E, 16,
        0x27
    };
    Bus bus(testCode);
    dynaRec dyna(bus);
    dyna.dispatch(0);
}
