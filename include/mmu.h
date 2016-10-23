#ifndef MMU_H
#define MMU_H

#include "cart.h"
#include "gb_registers.h"

#define HIGH_REGISTER_COUNT 0x100
#define HIGH_REGISTER_OFFSET 0xFF00

// An Mmu maps memory addresses to hardware registers including:
//   - cartridge ram
//   - main memory
//   - special purpose registers
//   - video ram
//
// The subsystems of the console communicate through this memory, they
// don't talk directly to each other
class Mmu {
public:
    Mmu(Cartridge *newCart);
    Mmu();
    // ~Mmu not needed since there's no dynamically allocated memory
    u8 &get(u16 addr);
    u16 get16(u16 addr);

    void set(u16 addr, u8 val);

    void zeroAllMemory();
    u8 &operator[](u16 index);

    void loadCart(Cartridge *newCart);

    u8 displayRamBank1[DISPLAY_RAM_STOP-DISPLAY_RAM_START] = {0};
private:
    u8 mem[MEMORY_SIZE] = {0}; // Waste some small amount of memory
    Cartridge *cart;
};


#endif //MMU_H
