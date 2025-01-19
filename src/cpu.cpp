#include <dynarec.hpp>
#include <vector>

int main()
{
    std::vector<uint8_t> testCode;
    testCode.resize(0x200);
    std::vector<uint8_t> Code1 = {
        0x3E, 0xA, //LD A, 0xA
        0xD6, 0xA, //SUB A, 0xA
        0xCD, 100, 0,
        0x3E, 1,
        0x10
    };

    std::vector<uint8_t> code2 = {
        0xF5, //PUSH AF
        0xC6, 0xF, //ADD A, 0xF
        //0xF1,
        0xC1,
        0xC9
    };

    for(int i = 0; i < Code1.size(); i++){
        testCode[i] = Code1[i];
    }

    for(int i = 0; i < code2.size(); i++)
        testCode[i + 100] = code2[i];
    Bus bus(testCode);
    dynaRec dyna(bus);
    dyna.dispatch(0);
}
