#include <dynarec.hpp>



dynaRec::dynaRec(std::vector<uint8_t>& targetCode): targetCode(targetCode)
{

}

Cache dynaRec::buildCache(uint16_t targetStartingAddress)
{
    Cache cache;
    Translator translator(cache.x86, targetCode);
    cache.targetStartingAddress = targetStartingAddress;
    cache.targetEndingAddress = translator.getJMPAddress(targetStartingAddress);

    uint16_t programCounter = cache.targetStartingAddress;
    while(programCounter <= cache.targetEndingAddress){
        translator.translateCurrentInstruction(programCounter);
    }
    return cache;
}

std::vector<uint8_t>& dynaRec::getCache(uint16_t targetStartingAddress)
{
    if(!cache.contains(targetStartingAddress))
        cache[targetStartingAddress] = buildCache(targetStartingAddress);
    return cache[targetStartingAddress].x86;
}
