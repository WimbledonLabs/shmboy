#include "cpu.h"

void GbMemory::zeroAllMemory() {
    for (int i=0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
}

void GbMemory::set(int index, u8 value) {
    loc(index) = value;
}

u8 GbMemory::get(int index) {
    return loc(index);
}

u8& GbMemory::loc(int index) {
    if ( DISPLAY_RAM_START <= index && index <= DISPLAY_RAM_STOP) {
        if (get(VBK)) {
            return displayRamBank1[index-DISPLAY_RAM_START];
        } else {
            return memory[index];
        }
    } else if (false) {
        return memory[0];
    } else {
        return memory[index];
    }
}

void Cpu::reset() {
    mem.set(P1,   0);
    mem.set(SC,   0);
    mem.set(TIMA, 0);
    mem.set(TAC,  0);
    mem.set(IE,   0);
    mem.set(LCDC, 0); //TODO actual value: $83 BG/OBJ ON, LCDC OPERATION;
    mem.set(SCY,  0);
    mem.set(SCX,  0);
    mem.set(LYC,  0);
    mem.set(WY,   0);
    mem.set(WX,    0);
    mem.set(IE,   0);// DI; //TODO DI = disable interrupt instruction

    mem.set(IF, 0); // needed? - SHM

    mem.set(VBK, 0); // CGB only, do anyways
}

Cpu::Cpu() {
    ppu = new Ppu(this,
                  &mem);
    reset();
}

Cpu::~Cpu() {
    delete ppu;
}
