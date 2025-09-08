#include "bus.hpp"
#include "dynarec.hpp"
#include "translator.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>


TEST(DAA, ADD)
{
    std::vector<uint8_t> testCode = {
        0x3E, 0x15,     // LD A, 0x15
        0x06, 0x27,    //  LD B, 0x27
        0x80,         //   ADD A, B
        0x27,        //    DAA
        0x10,       //     STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), 0x42);
}

TEST(DAA, SUB)
{
    std::vector<uint8_t> testCode = {
        0x3E, 0x42,     // LD A, 0x42
        0x06, 0x27,    //  LD B, 0x27
        0x90,         //   SUB A, B
        0x27,        //    DAA
        0x10,       //     STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), 0x15);
}
