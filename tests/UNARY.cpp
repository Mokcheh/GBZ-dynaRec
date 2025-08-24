#include "bus.hpp"
#include "dynarec.hpp"
#include "translator.hpp"
#include <cstdint>
#include <gtest/gtest.h>
#include <vector>


constexpr uint8_t registerContent = 0x08;

TEST(UNARY, INC_A){
    std::vector<uint8_t> testCode = {
        0x06, registerContent,  // LD B, 0x08
        0x04,                  //  INC B
        0x10                  //   STOP 
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::B)), registerContent + 1);

}

TEST(UNARY, DEC_U){
    std::vector<uint8_t> testCode = {
        0x06, registerContent,  // LD B, 0x08
        0x05,                  //  DEC B
        0x10                  //   STOP 
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::B)), registerContent - 1);
}