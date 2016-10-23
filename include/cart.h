#ifndef CART_H
#define CART_H

#include <iostream>
#include "shmboy_common.h"

const u8 SUPPORTED_CARTRIDGE_TYPES[] { 0x00 };

// ROM header information locations
// Information for the Game Boy CPU Manual:
//   http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
#define CODE_EXECUTION_START_ADDR 0x0100

#define TITLE_START_ADDR 0x0134
#define MAX_TITLE_LENGTH 16

#define CGB_MODE_ADDR 0x0143
#define CGB_MODE_VALUE 0x80

#define HIGH_LICENSEE_CODE_ADDR 0x0144 // Unused
#define LOW_LICENSEE_CODE_ADDR 0x0145 // Unused

#define SGB_MODE_ADDR 0x0146 // Unused
#define SGB_MODE_VALUE 0x03 // Unused

#define CART_TYPE_ADDR         0x0147
#define ROM_SIZE_ADDR          0x0148
#define RAM_SIZE_ADDR          0x0149
#define REGION_CODE_ADDR       0x014A // Unused
#define OLD_LICENSEE_CODE_ADDR 0x014B // Unused
#define MASK_ROM_VERSION_ADDR  0x014C // Unused
#define COMPLEMENT_CHECK_ADDR  0x014D // Unused
#define CHECKSUM_START_ADDR    0x014E // Unused
#define CHECKSUM_LENGTH 2

#define BIG_NUMBER 50000000

#define MAX_ROM_SIZE BIG_NUMBER
#define MAX_RAM_SIZE BIG_NUMBER

#define BANK_SIZE 0x4000

bool isCartridgeTypeSupported(u8 cartType);
const char *cartType(u8 cartType);
const char *ramSizeName(u8 romSize);
const char *romSizeName(u8 romSize);
const char *destinationCodeName(u8 code);

typedef enum {
    CARTRIDGE_LOADED = 0,
    CARTRIDGE_NOT_LOADED,
    FILE_LOAD_ERROR,
} CartridgeStatus;

class Cartridge {
public:
    Cartridge(std::istream *romFile);
    Cartridge();
    ~Cartridge();
    u8 *getTitle();
    bool isColorMode();
    u8 getCartridgeType();
    const char* getCartridgeTypeName();
    u32 getRomSize();
    u32 getRamSize();
    u8 getRegionCode();
    CartridgeStatus getCartridgeStatus();

    u8 *fetch(u16 pc);
    void cartRomMode(u16 addr, u8 val);

    // rom stores the entire contents of the cartridge
    u8 *rom;

    // ram, some carts have it and others don't, I still need to figure out
    // how the harwdare generally treats ram...
    u8 *ram;

    // The following banks point to memory locations within the rom
    // bank0 always points to the start of the rom
    u8 *bank0;

    // bank1 is indexed by [0x4000-0x8000)
    u8 *bank1;

private:

    u32 romSize = 0;
    u32 ramSize = 0;

    CartridgeStatus status = CARTRIDGE_NOT_LOADED;
};

#endif // CART_H
