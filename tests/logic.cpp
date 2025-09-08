#include <cstdint>
#include <gtest/gtest.h>
#include <vector>
#include "dynarec.hpp"
#include "translator.hpp"

TEST(LOGIC, OR_A_U8)
{
    constexpr uint8_t A = 0xF2;
    constexpr uint8_t val = 0xA0; 
    std::vector<uint8_t> testCode = {
        0x3E, A,    // LD A, 0xF2
        0xF6, val, //  OR A, 0xA0
        0x10,     //   STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), A | val);
}

TEST(LOGIC, AND_A_U8)
{
    constexpr uint8_t A = 0xF2;
    constexpr uint8_t val = 0xA0; 
    std::vector<uint8_t> testCode = {
        0x3E, A,    // LD A, 0xF2
        0xE6, val, //  AND A, 0xA0
        0x10,     //   STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), A & val);
    EXPECT_EQ(dynarec.getFlags(), static_cast<uint8_t>(FLAG::HALFCARRY));
}




TEST(LOGIC, XOR_A_U8)
{
    constexpr uint8_t A = 0xF2;
    constexpr uint8_t val = 0xA0; 
    std::vector<uint8_t> testCode = {
        0x3E, A,    // LD A, 0xF2
        0xEE, val, //  XOR A, 0xA0
        0x10,     //   STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), A ^ val);
}


TEST(LOGIC, CP_A_U8)
{
    constexpr uint8_t A = 0xF2;
    constexpr uint8_t val = 0xA0; 
    std::vector<uint8_t> testCode = {
        0x3E, A,    // LD A, 0xF2
        0xFE    , val, //  XOR A, 0xA0
        0x10,     //   STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    // TODO: TEST THE FLAGS
}