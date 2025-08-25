#include <cstdint>
#include <gtest/gtest.h>
#include <vector>
#include "bus.hpp"
#include "dynarec.hpp"
#include "translator.hpp"


TEST(CALL, u16){
    std::vector<uint8_t> testCode;
    testCode.resize(0x200);
    std::vector<uint8_t> testCode1 = {
        0xCD, 0x00, 0x01, // call 0x0100
    };
    std::vector<uint8_t> testCode2 = {
        0x06, 0xA, //LD B, 0xA
        0x10,
    };
    for (int i = 0; i < testCode1.size(); i++) 
        testCode[i] = testCode1[i];
    for (int i = 0; i < testCode2.size(); i++)
        testCode[i + 0x100] = testCode2[i];
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::B)), 0xA);
}

TEST(RETURN, RET){
    std::vector<uint8_t> testCode;
    testCode.resize(0x200);
    std::vector<uint8_t> testCode1 = {
        0xCD, 0x00, 0x01,  // call 0x0100
        0x21, 0x12, 0xF2, //  LD HL, 0xF212
        0x10,            //   STOP
    };
    std::vector<uint8_t> testCode2 = {
        0x06, 0xA, // LD B, 0xA
        0xC9,     //  RET
    };
    for (int i = 0; i < testCode1.size(); i++) 
        testCode[i] = testCode1[i];
    for (int i = 0; i < testCode2.size(); i++)
        testCode[i + 0x100] = testCode2[i];
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::B)), 0xA);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR16(gbz80::HL)), 0xF212);
}

