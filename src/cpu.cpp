#include <dynarec.hpp>
#include <vector>

int main()
{
    std::vector<uint8_t> testCode;
    testCode.resize(0x200);
    std::vector<uint8_t> Code2 = {
        0x09,
        0xC9, //RET
    };
    std::vector<uint8_t> Code1 = {
        0x01, 0x10, 0x14,
        0xCD, 0x00, 0x01, // call 0x0100
        0x06, 0xA, //LD B, 0xA
        0x10,
    };

    for(int i = 0; i < Code1.size(); i++){
        testCode[i] = Code1[i];
    }

    for(int i = 0; i < Code2.size(); i++){
        testCode[i + 0x100] = Code2[i];
    }
    Bus bus(testCode);
    dynaRec dyna(bus);
    dyna.dispatch(0);
}
