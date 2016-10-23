#include "cpu.h"

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

    pc = 0x100;
    sp = 0xDEAD; //TODO is it zero, or does it count the other way?
}

Cpu::Cpu() {
    ppu = new Ppu(this, &mem);
    reset();
}

void Cpu::loadCart(Cartridge *cart) {
    mem.loadCart(cart);
}

Cpu::~Cpu() {
    delete ppu;
}

OpCode Cpu::fetch(int pc) {
    OpCode op;
    op.value = mem[pc];
    return op;
}

u8 Cpu::fetchImm8(int pc) {
    return mem[pc+1];
}

u16 Cpu::fetchImm16(int pc) {
    return mem.get16(pc+1);
}

void Cpu::step() {
    OpCode op = fetch(pc);
    u8 imm8 = fetchImm8(pc);
    u16 imm16 = fetchImm16(pc);
    pc++; // TODO increment more for multi-byte instructions
    execute(op, imm8, imm16);
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

void Cpu::execute(OpCode op, u8 imm8, u16 imm16) {
    #include "opcodeExecSwitch.c"
}
