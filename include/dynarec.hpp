#pragma once
#include "x86_64Emitter.hpp"
#include <unordered_map>
#include <translator.hpp>
#include <array>
#include <vector>
#include <cstdint>
#include <memory>

class Cache;

class dynaRec{
public:
    dynaRec(Bus& bus);
    Cache& getCache(uint16_t targetStartingAddress);
    void dispatch(uint16_t programCounter);
    uint8_t getRegister(x86_8 reg) const;
    uint16_t getRegister(x86_16 reg) const;
    uint8_t getFlags() const;
private:
    Bus& bus;
    std::unordered_map<uint16_t, Cache> cache;
    void buildCache(Cache& cache, uint16_t targetStartingAddress);
    std::array<uint16_t, 6> registerState;
    uint16_t nextAddress;
};

struct Address {
    uint16_t address;
    bool isSet;
};

class Cache{
public:
    Cache();
    ~Cache();
    void run(const uint16_t* state);
    std::vector<uint8_t> x86;
    void setStartingAddress(uint16_t adr);
    void setEndingAddress(uint16_t adr);
    uint16_t getStartingAddress();
    uint16_t getEndingAddress();
    void setCycleCount(uint32_t cycles);
    uint32_t getCycleCount();
    uint16_t getJumpAddress();
    bool isJumpSet();
    void setRuntimeReturnAddress(std::shared_ptr<uint16_t> adr);
    void setJumpAddress(uint16_t adr);
private:
    std::shared_ptr<uint16_t> runtimeReturnAddress;
    void* generateExecutableCode(const uint16_t* state);
    void* runX86;
    Address start, end, jump;
    uint32_t sizeRounded;
    uint32_t cycles;
};

enum class FLAG : uint8_t {
    ALL_CLEAR = 0,
    ZERO = 0b10000000,
    SUB = 0b01000000,
    HALFCARRY = 0b00100000,
    CARRY = 0b00010000
};