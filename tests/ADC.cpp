#include <cstdint>
#include <gtest/gtest.h>
#include <vector>
#include "dynarec.hpp"
#include "translator.hpp"


TEST(ADC, A_R8){
    constexpr uint8_t val = 5;
    constexpr uint8_t addedVal = 10;
    constexpr uint8_t carry = 1;
    {
        constexpr uint8_t result = val + addedVal + carry;
        std::vector<uint8_t> testCode = {
            0x37,      // SCF
            0x06, 10, //  LD B, 10
            0x3E, 5, //   LD A, 5
            0x88,   //    ADC A, B
            0x10,  //     STOP
        };
        Bus bus(testCode);
        dynaRec dynarec(bus);
        dynarec.dispatch(0);
        ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result);
        EXPECT_EQ(dynarec.getFlags(), static_cast<uint8_t>(FLAG::HALFCARRY));
    }

    {
        constexpr uint8_t result = val + addedVal;
        std::vector<uint8_t> testCode = {
            0x3F,            // CCF
            0x06, addedVal, //  LD B, 10
            0x3E, val,     //   LD A, 5
            0x88,         //    ADC A, B
            0x10,        //     STOP
        };
        Bus bus(testCode);
        dynaRec dynarec(bus);
        dynarec.dispatch(0);
        ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result);
        EXPECT_EQ(dynarec.getFlags(), static_cast<uint8_t>(FLAG::ALL_CLEAR));
    }
}


TEST(ADC, A_U8){
    constexpr uint8_t val = 5;
    constexpr uint8_t addedVal = 10;
    constexpr uint8_t carry = 1;
    {
        constexpr uint8_t result = val + addedVal + carry;
        std::vector<uint8_t> testCode = {
            0x37,               // SCF
            0x3E, val,         //  LD A, val
            0xCE, addedVal,   //   ADC A, addedVal
            0x10,            //    STOP
        };
        Bus bus(testCode);
        dynaRec dynarec(bus);
        dynarec.dispatch(0);
        ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result);
        EXPECT_EQ(dynarec.getFlags(), static_cast<uint8_t>(FLAG::HALFCARRY));   
    }

    {
        constexpr uint8_t result = val + addedVal;
        std::vector<uint8_t> testCode = {
            0x3F,            //  CCF
            0x3E, val,      //   LD A, val
            0xCE, addedVal,//    ADC A, addedVal
            0x10,         //     STOP
        };
        Bus bus(testCode);
        dynaRec dynarec(bus);
        dynarec.dispatch(0);
        ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result);
        EXPECT_EQ(dynarec.getFlags(), static_cast<uint8_t>(FLAG::ALL_CLEAR));   
    }
}


