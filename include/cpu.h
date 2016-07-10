#ifndef CPU_H
#define CPU_H

#include "shmboy_common.h"
#include "ppu.h"

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
    int sp;
    int pc;

    GbMemory mem;
    Ppu *ppu;

    Cpu();
    ~Cpu();

    void reset();
    OpCode fetch(int pc);
    void execute(OpCode op);
};


#endif // CPU_H
