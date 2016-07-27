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

u8& GbMemory::operator[](u16 index) {
    return loc(index);
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
    mem.set(WX,   0);
    mem.set(IE,   0);// DI; //TODO DI = disable interrupt instruction

    mem.set(IF, 0); // needed? - SHM

    mem.set(VBK, 0); // CGB only, do anyways
}

Cpu::Cpu() {
    ppu = new Ppu(this, &mem);
    reset();
}

Cpu::~Cpu() {
    delete ppu;
}

OpCode Cpu::fetch(int pc) {
    // TODO
}

u8 Cpu::fetchImm8(int pc) {
    // TODO
}

u16 Cpu::fetchImm16(int pc) {
    // TODO
}

bool Cpu::flag(u8 f) {
    // TODO
}

bool Cpu::stop() {
    // TODO
}

bool Cpu::checkHalt(u8 op) {
    // TODO
}

void Cpu::halt() {
    // TODO
}

u8 Cpu::bcd(u8) {
    // TODO
}

u8 Cpu::alu(u8 dest, u8 src, u8 aluOp) {
    // TODO
}

u8 Cpu::rot(u8 dest, u8 dir, u8 modifyFlags) {
    // TODO
}

void Cpu::rst() {
    // TODO
}

void Cpu::cb() {
    // TODO
}

void Cpu::execute(OpCode op) {
    u8 imm8 = fetchImm8(pc+1);
    u16 imm16 = fetchImm16(pc+1);
    #include "opcodeExecSwitch.c"
}
