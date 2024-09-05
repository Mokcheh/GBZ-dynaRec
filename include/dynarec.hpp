#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <translator.hpp>

struct Cache;

class dynaRec{
public:
    dynaRec(std::vector<uint8_t>& targetCode);
    std::vector<uint8_t>& getCache(uint16_t targetStartingAddress);
private:
    std::unordered_map<uint16_t, Cache> cache;
    std::vector<uint8_t>& targetCode;
    Cache buildCache(uint16_t targetStartingAddress);
};


struct Cache{
    std::vector<uint8_t> x86;
    uint16_t targetStartingAddress;
    uint16_t targetEndingAddress;
};
