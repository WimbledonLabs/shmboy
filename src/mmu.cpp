#include "mmu.h"

Mmu::Mmu(Cartridge *newCart) {
    loadCart(newCart);
}

Mmu::Mmu() {
    cart = nullptr;
}

void Mmu::loadCart(Cartridge *newCart) {
    cart = newCart;
    zeroAllMemory();
}

u8 &Mmu::get(u16 addr) {
    if (addr < 0x4000) { // ROM bank 0
        return cart->bank0[addr];
    } else if (addr < 0x8000) { // Switchable ROM bank
        return cart->bank1[addr];
    } else if (addr < 0xA000) { // Video RAM
        if (get(VBK)) {
            return displayRamBank1[addr-DISPLAY_RAM_STOP];
        }
        return mem[addr];
    } else if (addr < 0xC000) { // Switchable RAM bank
        return cart->ram[addr];
    } else if (addr < 0xE000) { // Internal RAM bank
        return mem[addr];
    } else if (addr < 0xF000) { // Echo of internal RAM
        return mem[addr - (0xE000-0xC000)];
    } else { // Special purpose registers (most not used)
        return mem[addr];
    }
}

u16 Mmu::get16(u16 addr) {
    return get(addr+1) << 8 | get(addr);
}

void Mmu::set(u16 addr, u8 val) {
    if (addr < 0x8000) { // Writin to the ROM is used to switch memory banks
        cart->cartRomMode(addr, val); // TODO better name for method
    } else if (addr < 0xA000) { // Video RAM
        mem[addr] = val;
    } else if (addr < 0xC000) { // Cartridge Switchable RAM bank
        cart->ram[addr] = val;
    } else if (addr < 0xE000) { // Internal RAM bank
        mem[addr] = val;
    } else if (addr < 0xF000) { // Echo of internal RAM
        mem[addr - (0xE000-0xC000)] = val;
    } else { // Special purpose registers (most space not used)
        mem[addr] = val;
    }
}

void Mmu::zeroAllMemory() {
    for (int i=0; i < MEMORY_SIZE; i++) {
        mem[i] = 0;
    }
    for (int i=0; i < DISPLAY_RAM_STOP-DISPLAY_RAM_START; i++) {
        mem[i] = 0;
    }
}

u8 &Mmu::operator[](u16 index) {
    return get(index);
}

