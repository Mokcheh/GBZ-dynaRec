#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <translator.hpp>
#include <array>

class Cache;

class dynaRec{
public:
    dynaRec(std::vector<uint8_t>& targetCode);
    Cache& getCache(uint16_t targetStartingAddress);
    void dispatch(uint16_t programCounter);
private:
    std::unordered_map<uint16_t, Cache> cache;
    std::vector<uint8_t>& targetCode;
    void buildCache(Cache& cache, uint16_t targetStartingAddress);
    std::array<uint16_t, 6> registerState;
};

class Cache{
public:
    Cache():runX86(nullptr){};
    ~Cache();
    std::vector<uint8_t> x86;
    uint16_t targetStartingAddress;
    uint16_t targetEndingAddress;
    uint32_t cycles;
    void run(const uint16_t* state);
private:
    void* generateExecutableCode(const uint16_t* state);
    void* runX86;
    size_t sizeRounded;
};
