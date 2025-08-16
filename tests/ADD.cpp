#include <cstdint>
#include <gtest/gtest.h>
#include <vector>
#include "dynarec.hpp"
#include "tester_boilerplate.hpp"
#include "translator.hpp"


TEST(ADD, A_U8){
    {
        constexpr uint8_t val = 0xF1;
        constexpr uint8_t addedVal = 0x0F;
        constexpr uint8_t result = static_cast<const uint8_t>(val + addedVal);
        std::vector<uint8_t> testCode = {
            0x3E, val,       // LD A, 0xF1
            0xC6, addedVal, //  ADD A, val
            0x10,          //   STOP 
        };    
        Bus bus(testCode);
        dynaRec dynarec(bus);
        dynarec.dispatch(0);
        ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result)
        << "EXPECTED : A = " << (int)result << " Result : A = " 
        << (int)dynarec.getRegister(Translator::mapR8(gbz80::A));
        // 0xF1 + 0x0F should trigger Z H C flags and reset N, matching the startup flags
        EXPECT_EQ(dynarec.getFlags(), initialFlags);
    }

    {
        constexpr uint8_t val = 10;
        constexpr uint8_t addedVal = 13;
        constexpr uint8_t result = val + addedVal;
        std::vector<uint8_t> testCode = {
            0x3E, val,       // LD A, val
            0xC6, addedVal, //  ADD A, addedVal
            0x10,          //   STOP 
        };    
        Bus bus(testCode);
        dynaRec dynarec(bus);
        dynarec.dispatch(0);
        ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result)
        << "EXPECTED : A = " << (int)result << " Result : A = " 
        << (int)dynarec.getRegister(Translator::mapR8(gbz80::A));
        // 13 + 10 should trigger H flag
        EXPECT_EQ(dynarec.getFlags(), (uint8_t)FLAG::HALFCARRY);
    }
    
}

TEST(ADD, A_R8){
    constexpr uint8_t val = 5;
    constexpr uint8_t addedVal = 10;
    constexpr uint8_t result = val + addedVal;
    std::vector<uint8_t> testCode = {
        0x06, addedVal,         // LD B, addedVal
        0x3E, val,             //  LD A, val
        0x80,                 //   ADD A, B
        0x10,                //    STOP 
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result)
    << "EXPECTED : A = " << 15 << " Result : A = " 
    << (int)dynarec.getRegister(Translator::mapR8(gbz80::A));
    EXPECT_EQ(dynarec.getFlags(), static_cast<uint8_t>(FLAG::ALL_CLEAR));
}


TEST(ADD, A_HL_PTR){
    constexpr uint8_t val = 0xF1;
    constexpr uint8_t addedVal = 0x0F;
    constexpr uint8_t result = static_cast<uint8_t>(val + addedVal);
    std::vector<uint8_t> testCode = {
        0x3E, val,       // LD A, 0xF1
        0x21, 0x10, 0xF2, //LD HL, 0xF210
        0x86, // ADD A, [HL]
        0x10,          //   STOP 
    };    
    Bus bus(testCode);
    bus.memory[0xF210] = addedVal;
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), result)
    << "EXPECTED : A = " << (int)result << " Result : A = " 
    << (int)dynarec.getRegister(Translator::mapR8(gbz80::A));
    // 0xF1 + 0x0F should trigger Z H C flags and reset N, matching the startup flags
    EXPECT_EQ(dynarec.getFlags(), initialFlags);
}