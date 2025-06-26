#pragma once

#include "translator.hpp"
#include <array>
#include <cstdint>



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