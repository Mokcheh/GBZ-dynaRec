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

TEST(STACK, PUSH_AF){
    constexpr uint8_t registerContent = 0x42;
    std::vector<uint8_t> testCode = {
        0x3E, registerContent, // LD A, 0x42
        0xF5, // PUSH AF
        0x10, //STOP
    };
        Bus bus(testCode);
    dynaRec dynarec(bus);
    const uint16_t oldStackPtr = dynarec.getRegister(Translator::mapR16(gbz80::SP));
    dynarec.dispatch(0);
    const uint16_t contentOnStack = 
    bus.memory.at(dynarec.getRegister(Translator::mapR16(gbz80::SP))) |
    (bus.memory.at(dynarec.getRegister(Translator::mapR16(gbz80::SP)) + 1) << 8);
    EXPECT_EQ(contentOnStack , (registerContent << 8) | initialFlags);
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

TEST(STACK_PTR, ADD_SP_POSITIVE_8)
{
    std::vector<uint8_t> testCode = {
        0xE8, 2, // ADD SP, 2
        0x10,   //  STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    const uint16_t oldSP = dynarec.getRegister(Translator::mapR16(gbz80::SP)); 
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(
        Translator::mapR16(gbz80::SP)), 
        static_cast<uint16_t>(oldSP + 2)
    );
}

TEST(STACK_PTR, ADD_SP_NEGATIVE_8)
{
    std::vector<uint8_t> testCode = {
        0xE8, static_cast<uint8_t>(-2), // ADD SP, 2
        0x10,                          //  STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    const uint16_t oldSP = dynarec.getRegister(Translator::mapR16(gbz80::SP)); 
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(
        Translator::mapR16(gbz80::SP)), 
        static_cast<uint16_t>(oldSP - 2)
    );
}