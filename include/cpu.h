#ifndef CPU_H
#define CPU_H

#include "shmboy_common.h"
#include "ppu.h"

#define REGISTER_COUNT 8

#define REG_A 7
#define REG_B 0
#define REG_C 1
#define REG_D 2 // Multiple 5's ...?
#define REG_E 3
#define REG_H 4
#define REG_L 5

#define REG_F 6 // ???

#define REG_HL 5

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

    u8 &operator[](u16 loc);
};

typedef union {
    u8 value;
    struct {
        unsigned int digit7_7: 1;
        unsigned int digit6_6: 1;
        unsigned int digit5_5: 1;
        unsigned int digit4_4: 1;
        unsigned int digit3_3: 1;
        unsigned int digit2_2: 1;
        unsigned int digit1_1: 1;
        unsigned int digit0_0: 1;
    };

    struct {
        unsigned int digit5_7: 3;
        unsigned int digit2_4: 3;
        unsigned int: 2;
    };

    struct {
        unsigned int: 3;
        unsigned int digit3_4: 2;
        unsigned int: 3;
    };

    struct {
        unsigned int digit6_7: 2;
        unsigned int digit4_5: 2;
        unsigned int digit2_3: 2;
        unsigned int digit0_1: 2;
    };

    struct {
        unsigned int:     3;
        unsigned int alu: 3;
        unsigned int:     2;
    };

    struct {
        unsigned int d2: 3;
        unsigned int d1: 3;
        unsigned int:    2;
    };

    struct {
        unsigned int:    4;
        unsigned int nC: 1;
        unsigned int:    3;
    };

    struct {
        unsigned int:    4;
        unsigned int sR: 1;
        unsigned int:    3;
    };

    struct {
        unsigned int:   3;
        unsigned int F: 2;
        unsigned int:   3;
    };

    struct {
        unsigned int:     3;
        unsigned int dir: 1;
        unsigned int R:   2;
        unsigned int:     2;
    };

} OpCode;

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
    void halt();
    u8 bcd(u8);
    u8 alu(u8 dest, u8 src, u8 aluOp);
    u8 rot(u8 dest, u8 dir, u8 modifyFlags);
    void rst();
    void cb();

    void reset();
    OpCode fetch(int pc);
    u8 fetchImm8(int pc);
    u16 fetchImm16(int pc);
    void execute(OpCode op);
};


#endif // CPU_H
