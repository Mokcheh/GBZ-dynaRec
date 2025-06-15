#include "bus.hpp"
#include "dynarec.hpp"
#include "translator.hpp"
#include <array>
#include <cstdint>
#include <gtest/gtest.h>
#include <ios>
#include <vector>

struct reg8{
    char name;
    gbz80::r index;
};

struct reg16{
    char name[3];
    gbz80::rp index;
};

constexpr std::array<reg8, 7> registers8 = {
    reg8{'A', gbz80::A}, reg8{'B', gbz80::B},
    reg8{'C', gbz80::C}, reg8{'D', gbz80::D},
    reg8{'E', gbz80::E}, reg8{'H', gbz80::H}, 
    reg8{'L', gbz80::L}
};

constexpr std::array<reg16, 4> registers16 = {
    reg16{"BC", gbz80::BC}, reg16{"DE", gbz80::DE},
    reg16{"HL", gbz80::HL}, reg16{"SP", gbz80::SP}
};

constexpr std::array<uint8_t, 7> u8 = {
    0x1A, 0x82, 0xCA, 0x88, 0x26, 0x05, 0xEC
};

constexpr uint8_t initialFlags = 0xB0;

TEST(LD, R8_U8){
    std::vector<uint8_t> testCode = {
        0x3E, u8[0], //LD A, u8[0]
        0x06, u8[1], //LD B, u8[1]
        0x0E, u8[2], //LD C, u8[2]
        0x16, u8[3], //LD D, u8[3]
        0x1E, u8[4], //LD E, u8[4]
        0x26, u8[5], //LD H, u8[5]
        0x2E, u8[6], //LD L, u8[6]
        0x10 //STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getFlags(), initialFlags);
    for (int i = 0; i < registers8.size(); i++) {
        EXPECT_EQ(dynarec.getRegister(Translator::mapR8(registers8[i].index)), u8[i])
        << "LD " << registers8[i].name << ", " << u8[i] <<  " : EXPECTED  " << registers8[i].name << " = " << u8[i] 
        << "; RESULT : " << registers8[i].name << "= 0x" << std::uppercase << std::hex 
        << (int)dynarec.getRegister(Translator::mapR8(registers8[i].index));
    }
}

TEST(LD, R8_R8){
    std::vector<uint8_t> testCode = {
        0x3E, u8[0], //LD A, u8[0]
        0x47, // LD B, A
        0x4F, // LD C, A
        0x57, // LD D, A
        0x5F, // LD E, A
        0x67, // LD H, A
        0x6F, // LD L, A
        0x10 // STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getFlags(), initialFlags);
    for (auto &r : registers8) {
        EXPECT_EQ(dynarec.getRegister(Translator::mapR8(r.index)), u8[0])
        << "LD " << r.name << ", " << u8[0] <<  " : EXPECTED  " << r.name << " = " << u8[0] 
        << "; RESULT : " << r.name << "= 0x" << std::uppercase << std::hex 
        << (int)dynarec.getRegister(Translator::mapR8(r.index));
    }
}


TEST(LD, R16_U16){
    std::vector<uint8_t> testCode = {
        0x01, u8[0], u8[6], // LD BC, 0xEC1A
        0x11, u8[1], u8[5], // LD DE, 0x0582
        0x21, u8[2], u8[4], // LD HL, 0x26CA
        0x31, u8[3], u8[3], // LD SP, 0x8888
        0x10 // STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getFlags(), initialFlags);
    for (int i = 0; i < registers16.size(); i++) {
        const uint16_t loadValue = u8[i] | (u8[u8.size() - i - 1] << 8);
        EXPECT_EQ(dynarec.getRegister(Translator::mapR16(registers16[i].index)), loadValue)
        << "LD " << registers16[i].name << ", " << loadValue << " : EXPECTED : " << registers16[i].name
        << " = " << loadValue << " RESULT : " << registers16[i].name << " = "
        << dynarec.getRegister(Translator::mapR16(registers16[i].index));
    }
}

TEST(LD, U16_PTR_A){
    std::vector<uint8_t> testCode = {
        0x3E, u8[0], //LD A, u8[0]
        0xEA, 0x10, 0x1A, // LD [0x1A10], A
        0x10 // STOP
    };
    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getFlags(), initialFlags);
    ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), u8[0]);
    EXPECT_EQ(bus.memory.at(0x1A10), dynarec.getRegister(Translator::mapR8(gbz80::A)));
}


TEST(LD, A_U16_PTR){
    std::vector<uint8_t> testCode = {
        0xFA, 0x10, 0xA1, // LD A, [0xA110]
        0x10 // STOP
    };
    Bus bus(testCode);
    bus.memory[0xA110] = u8[0];
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getFlags(), initialFlags);
    EXPECT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), u8[0]);
}

TEST(LD, LDH_C_A){
    std::vector<uint8_t> testCode = {
        0x3E, u8[2], // LD A, u8[2]
        0xE0, u8[0], // LD [0xFF00 + u8[0]], A
        0x10, //STOP
    };

    Bus bus(testCode);
    dynaRec dynarec(bus);
    dynarec.dispatch(0);
    ASSERT_EQ(dynarec.getFlags(), initialFlags);
    ASSERT_EQ(dynarec.getRegister(Translator::mapR8(gbz80::A)), u8[2]);
    EXPECT_EQ(bus.memory.at(0xFF00 + u8[0]), u8[2]);
}