#include <array>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <cmath>
#include <dynarec.hpp>
#include <vector>

#ifdef __linux__
#include <unistd.h>
#include <sys/mman.h>

#elif _WIN32
#include <windows.h>
#include <memoryapi.h>

#else
#error Unsupported OS
#endif


dynaRec::dynaRec(Bus& bus) :
    bus(bus)
{
    registerState.fill(0);
    registerState[0] = 0xB0 << 8;
}

void Cache::setRuntimeReturnAddress(std::shared_ptr<uint16_t> adr)
{
    runtimeReturnAddress = adr;
}

void dynaRec::buildCache(Cache& cache, uint16_t targetStartingAddress)
{
    Translator translator(cache.x86, targetStartingAddress, bus);
    cache.setStartingAddress(targetStartingAddress);
    translator.translateBlock();
    cache.setEndingAddress(translator.blockProgramCounter);
    if(translator.getReturnAddress() != nullptr)
        cache.setRuntimeReturnAddress(translator.getReturnAddress());
    else if(translator.getJumpAddress() != 0xFFFF)
        cache.setJumpAddress(translator.getJumpAddress());
    cache.setCycleCount(translator.cyclesPassed);
}

Cache& dynaRec::getCache(uint16_t targetStartingAddress)
{
    if(!cache.contains(targetStartingAddress))
        buildCache(cache[targetStartingAddress], targetStartingAddress);
    return cache[targetStartingAddress];
}

void dynaRec::dispatch(uint16_t programCounter)
{
    Cache& current = getCache(programCounter);
    current.run(registerState.data());
    if(current.isJumpSet())
        dispatch(current.getJumpAddress());
}

std::vector<uint8_t> generateX64InitCode(const uint16_t* state){
    //Register states are loaded from an array before execution, saved after.
    std::vector<uint8_t> init = {
        0x53, //push rbx
        0x54, //push rsp
        0x55, //push rbp
        0x48, 0xBD, 0, 0, 0, 0, 0, 0, 0, 0, //movabs rbp, &state
        0x48, 0xC7, 0xC1, 0x00, 0x00, 0x00, 0x00, //mov rcx ,0
        0x48, 0xC7, 0xC3, 0x00, 0x00, 0x00, 0x00, //mov rbx ,0
        0x48, 0xC7, 0xC2, 0x00, 0x00, 0x00, 0x00, //mov rdx ,0
        0x48, 0xC7, 0xC6, 0x00, 0x00, 0x00, 0x00, //mov rsi ,0
        0x48, 0xC7 ,0xC7, 0x00, 0x00, 0x00, 0x00, //mov rdi ,0
        0x66, 0x8B, 0x45, 0x00, //mov ax, [rbp+0x0]
        0x66, 0x8B, 0x5D, 0x02, //mov bx, [rbp+0x2]
        0x66, 0x8B, 0x55, 0x04, //mov dx, [rbp+0x4]
        0x66, 0x8B, 0x75, 0x06, //mov si, [rbp+0x6]
        0x66, 0x8B, 0x4D, 0x08, //mov cx, [rbp+0x8]
        0x66, 0x8B, 0x7D, 0x0A, //mov di, [rbp+0xa]
    };
    //64bit Address to the array is injected using memcpy to avoid manually splitting it into 8bit blocks.
    memcpy(init.data() + 5, &state, 8);
    return init;
}

std::vector<uint8_t> generateX64CleanUpCode(const uint16_t* state){
    std::vector<uint8_t> cleanup = {
        0x48, 0xBD, 0, 0, 0, 0, 0, 0, 0, 0, //movabs rbp, &state
        0x66, 0x89, 0x45, 0x00, //mov [rbp+0x0], ax
        0x66, 0x89, 0x5D, 0x02, //mov [rbp+0x2], bx
        0x66, 0x89, 0x55, 0x04, //mov [rbp+0x4], dx
        0x66, 0x89, 0x75, 0x06, //mov [rbp+0x6], si
        0x66, 0x89, 0x4D, 0x08, //mov [rbp+0x8], cx
        0x66, 0x89, 0x7D, 0x0A, //mov [rbp+0xa], di
        0x5D, //pop rbp
        0x5C, //pop rsp
        0x5B, //pop rbx
        0xC3 //ret
    };
    memcpy(cleanup.data() + 2, &state, 8);
    return cleanup;
}



#ifdef __linux__
void* Cache::generateExecutableCode(const uint16_t* state)
{
    size_t pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize == -1) /*Page size is typically 4kb on linux.*/
    {
        std::cout << "Failed to get the page size!\n";
        exit(EXIT_FAILURE);
    }
    auto init = generateX64InitCode(state);
    auto cleanup = generateX64CleanUpCode(state);
    /*
     * mmap takes the size as a multiple of the page size.
     * So rounding the code size to the next largest multiple of 4kb is necessary.
    */
    this->sizeRounded =
        std::ceil((float)(x86.size() + init.size() + cleanup.size()) / pageSize) * pageSize;
    void* x86CodeRegion = mmap(
        nullptr,
        sizeRounded,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANON,
        -1,
        0
    );
    if (!x86CodeRegion)
    {
        std::cout << "Failed to initialize the code region!\n";
        exit(EXIT_FAILURE);
    }
    memcpy(x86CodeRegion, init.data(), init.size());
    memcpy((uint8_t*)x86CodeRegion + init.size(), x86.data(), x86.size());
    memcpy((uint8_t*)x86CodeRegion + init.size() + x86.size(), cleanup.data(), cleanup.size());
    if (mprotect(x86CodeRegion, sizeRounded, PROT_READ | PROT_EXEC) == -1)
    {
        std::cout << "Failed to make the code region executable!";
        exit(EXIT_FAILURE);
    }
    return x86CodeRegion;
}


Cache::~Cache()
{
    munmap(this->runX86, this->sizeRounded);
}


#elif _WIN32
void* Cache::generateExecutableCode(const uint16_t* state)
{
    auto init = generateX64InitCode(state);
    auto cleanup = generateX64CleanUpCode(state);
    sizeRounded = x86.size() + init.size() + cleanup.size();
    LPVOID x86CodeRegion = VirtualAlloc(NULL, sizeRounded, MEM_COMMIT, PAGE_READWRITE);
    if (!x86CodeRegion)
    {
        std::cout << "Failed to initialize the code region!\n";
        exit(EXIT_FAILURE);
    }

    memcpy(x86CodeRegion, init.data(), init.size());
    memcpy((uint8_t*)x86CodeRegion + init.size(), x86.data(), x86.size());
    memcpy((uint8_t*)x86CodeRegion + init.size() + x86.size(), cleanup.data(), cleanup.size());
    DWORD oldProtection; /*Whatever that is*/
    VirtualProtect(x86CodeRegion, sizeRounded, PAGE_EXECUTE_READ, &oldProtection);
    return (void*)x86CodeRegion;
}


Cache::~Cache()
{
    VirtualFree(runX86, 0, MEM_RELEASE);
}
#endif

Cache::Cache():
        runX86(nullptr), sizeRounded(0), cycles(0)
{
    start.address = end.address = jump.address = 0;
    start.isSet = end.isSet = jump.isSet = false;
    runtimeReturnAddress = nullptr;
}

void Cache::run(const uint16_t* state)
{
    if(runX86 == nullptr)
        runX86 = generateExecutableCode(state);
    ((void(*)()) runX86)();
    if(runtimeReturnAddress && !jump.isSet)
        setJumpAddress(*runtimeReturnAddress);
}

void Cache::setStartingAddress(uint16_t adr)
{
    try{
        if(start.isSet)
            throw std::runtime_error("Starting address has already been set!");
        start.address  = adr;
        start.isSet = true;
    }catch(const std::runtime_error& error){
        std::cout << "Cache Starting address : "
                  << start.address << " Error: " << error.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

void Cache::setEndingAddress(uint16_t adr)
{
    try{
        if(end.isSet)
            throw std::runtime_error("Ending address has already been set!");
        end.address  = adr;
        end.isSet = true;
    }catch(const std::runtime_error& error){
        std::cout << "Cache Ending address : "
                  << start.address << " Error: " << error.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

void Cache::setJumpAddress(uint16_t adr)
{
    try{
        if(jump.isSet)
            throw std::runtime_error("Jump address has already been set!");
        jump.address  = adr;
        jump.isSet = true;
    }catch(const std::runtime_error& error){
        std::cout << "Cache Jump address : "
                  << start.address << " Error: " << error.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

uint16_t Cache::getEndingAddress()
{
    return end.address;
}

uint16_t Cache::getStartingAddress()
{
    return start.address;
}

uint16_t Cache::getJumpAddress()
{
    return jump.address;
}

bool Cache::isJumpSet()
{
    return jump.isSet;
}

void Cache::setCycleCount(uint32_t cycles)
{
    try{
        if(this->cycles != 0)
            throw std::runtime_error("Cycle count has already been set!");
        this->cycles = cycles;
    }catch(const std::runtime_error& error){
        std::cout << "Cache cycle count : "
                  << start.address << " Error: " << error.what() << "\n";
        exit(EXIT_FAILURE);
    }
}

uint32_t Cache::getCycleCount()
{
    return cycles;
}
