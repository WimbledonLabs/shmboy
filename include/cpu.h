#ifndef CPU_H
#define CPU_H

#include "shmboy_common.h"
#include "ppu.h"

#define REGISTER_COUNT 8

#define REG_A 7
#define REG_B 0
#define REG_C 1
#define REG_D 5 // Multiple 5's ...?
#define REG_E 3
#define REG_H 4
#define REG_L 5

#define REG_HL

// Forward declarations
class Ppu;

class GbMemory {
public:
    u8 memory[MEMORY_SIZE];
    u8 displayRamBank1[DISPLAY_RAM_BANK_SIZE];
    u8 get(int index);
    void set(int index, u8 value);

    u8& loc(int index);
    void zeroAllMemory();
};

class Cpu {
public:
    u16 sp;
    u16 pc;
    u8 EI = 0;

    u8 reg[REGISTER_COUNT];
    u16 *regPair;

    GbMemory mem;
    Ppu *ppu;

    Cpu();
    ~Cpu();

    // Micro-op helper instructions
    bool flag(u8 f);
    bool stop();
    bool checkHalt(u8 op);
    u8 bcd();
    u8 alu(u8 *dest, u8 *src, u8 aluOp);
    u8 rot(u8 *dest, u8 dir, u8 modifyFlags);
    void cb();

    void reset();
    OpCode fetch(int pc);
    void execute(OpCode op);
};


#endif // CPU_H
