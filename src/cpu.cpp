#include <dynarec.hpp>
#include <vector>

int main()
{
    std::vector<uint8_t> testCode = {
        0x3E, 0,
        0xCB, 0x17,
        0xD6, 0,
        0x27
    };
    Bus bus(testCode);
    dynaRec dyna(bus);
    dyna.dispatch(0);
}
