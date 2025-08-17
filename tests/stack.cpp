#include <cstdint>
#include <gtest/gtest.h>
#include <vector>
#include "dynarec.hpp"
#include "tester_boilerplate.hpp"
#include "translator.hpp"

TEST(STACK, PUSH_RP)
{
    constexpr uint8_t low = 0x2C, high = 0xF8;
    constexpr uint16_t registerContent = (high << 8) | low;
    std::vector<uint8_t> testCode = {
        0x01, low, high, // LD BC, 0xF82C
        0xC5,           //  PUSH BC
        0x10,          //   STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    const uint16_t oldStackPtr = dynarec.getRegister(Translator::mapR16(gbz80::SP));
    dynarec.dispatch(0);
    const uint16_t contentOnStack = 
    bus.memory.at(dynarec.getRegister(Translator::mapR16(gbz80::SP))) |
    (bus.memory.at(dynarec.getRegister(Translator::mapR16(gbz80::SP)) + 1) << 8);
    EXPECT_EQ(contentOnStack , registerContent);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR16(gbz80::SP)), oldStackPtr - 2);
    EXPECT_EQ(dynarec.getFlags(), initialFlags);
}

TEST(STACK, POP_RP)
{
    constexpr uint8_t low = 0x2C, high = 0xF8;
    constexpr uint16_t registerContent = (high << 8) | low;
    std::vector<uint8_t> testCode = {
        0x11, low, high, // LD DE, 0xF82C
        0xD5,           //  PUSH DE
        0xC1,          //   POP BC
        0x10,         //    STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    const uint16_t initialStackPtr = dynarec.getRegister(Translator::mapR16(gbz80::SP));
    dynarec.dispatch(0);   
    EXPECT_EQ(dynarec.getRegister(Translator::mapR16(gbz80::BC)), registerContent);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR16(gbz80::SP)), initialStackPtr);
    EXPECT_EQ(dynarec.getFlags(), initialFlags);
}
