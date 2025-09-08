### Game Boy CPU

The original Game Boy (DMG-01) uses the **Sharp LR35902**, an 8-bit processor often nicknamed the "GB-Z80".  
It is a hybrid between the **Intel 8080** and the **Zilog Z80**, running at ~4.19 MHz.  
This custom design balances simplicity and efficiency, making it ideal for handheld gaming.

## Registers

The CPU has eight 8-bit registers (A, F, B, C, D, E, H, L). These can also be combined into four 16-bit register pairs (AF, BC, DE, HL), allowing the same hardware to be accessed as either 8-bit or 16-bit values.

In our case, the target cpu registers (GB-Z80) are mapped to lower 16 bit part the host cpu registers (x86_64) let's call it x86_16.

| Register | Size                | Purpose                    | x86_64 Register     |
|----------|---------------------|----------------------------|---------------------|
| AF       | 16-bit or two 8-bit | Accumulator (A) / flags (F)|AH                   |
| BC       | 16-bit or two 8-bit | Data/address               |CX                   |
| DE       | 16-bit or two 8-bit | Data/address               |BX                   |
| HL       | 16-bit or two 8-bit | Accumulator/address        |DX                   |
| SP       | 16-bit              | Stack pointer              |SI                   |
| PC       | 16-bit              | Program counter            |Handled in software  |

## Flags

CPU flags are also handled by the host flags. Though, there are some cases where handling it manually is necessary.

| Bit | Flag | Explanation      |
|-----|------|------------------|
| 7   | Z    | Zero flag        |
| 6   | N    | Subtraction flag |
| 5   | H    | Half Carry flag  |
| 4   | C    | Carry flag       |

Since there is no equivalent of the subtraction flag in x86_64, manually emulating it is necessary. In this project, the **DI** register is reserved for that purpose.


## Instructions

Instruction length varies between 1 and 4 bytes depending on the instruction.
Opcodes are split into 2 parts: unprefixed opcodes that live in the lower 256 bits, and 0xCB prefixed opcodes that live in the higher 256 bits. 

Opcodes can be followed by an immediate (Either 8 or 16bit). Which gives the instruction a total size of 1-4 bytes.

I find this [opcode table](https://izik1.github.io/gbops/) and this [instruction manual](https://rgbds.gbdev.io/docs/v0.9.4/gbz80.7) pretty helpful. And I've been using them throughout the development of this project.

## Dynamic recompilation

Unlike traditional interpreters, which fetch, decode and execute instructions one by one, dynamic recompilers achieve the same result by **grouping a block of instructions**, **translating that block to a block the host can execute**, and **executing it in one go**. This approach provides a performance uplift. Some portable languages such as Java, .Net Common Language Runtime and LuaJIT take advantage of it. While it's overkill for a simple CPU such as the GBZ80, it's a nice way to learn about dynamic recompilation.

## Goals
This project is used for educational purposes. It's written in C++20, GB-Z80 assembly and x86_64 assembly. Some instructions are yet to be implemented such as HALT, STOP and some relative call, jump instructions.

## What works and what doesn't
- Check unit tests
