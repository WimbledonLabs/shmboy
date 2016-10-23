#include "cart.h"

bool isCartridgeTypeSupported(u8 cartType) {
    for (int i=0; i < sizeof(SUPPORTED_CARTRIDGE_TYPES); i++) {
        if (cartType == SUPPORTED_CARTRIDGE_TYPES[i]) {
            return true;
        }
    }

    return false;
}

const char *cartType(u8 cartType) {
    switch (cartType) {
        case 0x00: return "ROM ONLY";
        case 0x01: return "ROM+MBC1";
        case 0x02: return "ROM+MBC1+RAM";
        case 0x03: return "ROM+MBC1+RAM+BATT";
        // case 0x4: return "NOT A THING 4";
        case 0x05: return "ROM+MBC2";
        case 0x06: return "ROM+MBC2+BATTERY";
        // case 0x7: return "NOT A THING 7";
        case 0x08: return "ROM+RAM";
        case 0x09: return "ROM+RAM+BATTERY";
        // case 0xa: return "NOT A THING A";
        case 0x0b: return "ROM+MMM01";
        case 0x0c: return "ROM+MMM01+SRAM";
        case 0x0d: return "ROM+MMM01+SRAM+BATT";
        // case 0xe: return "NOT A THING E";
        case 0x0f: return "ROM+MBC3+TIMER+BATT";
        case 0x10: return "ROM+MBC3+TIMER+RAM+BATT";
        case 0x11: return "ROM+MBC3";
        case 0x12: return "ROM+MBC3+RAM";
        case 0x13: return "ROM+MBC3+RAM+BATT";
        // case 0x14: return "ROM+";
        // case 0x15: return "ROM+";
        // case 0x16: return "ROM+";
        // case 0x17: return "ROM+";
        // case 0x18: return "ROM+";
        case 0x19: return "ROM+MBC5";
        case 0x1a: return "ROM+MBC5+RAM";
        case 0x1b: return "ROM+MBC5+RAM+BATT";
        case 0x1c: return "ROM+MBC5+RUMBLE";
        case 0x1d: return "ROM+MBC5+RUMBLE+SRAM";
        case 0x1e: return "ROM+MBC5+RUMBLE+SRAM+BATT";
        case 0x1f: return "POCKET CAMERA";
        case 0xfd: return "Bandai TAMA5";
        case 0xfe: return "Hudson HuC-3";
        case 0xff: return "Hudson HuC-1";
        default:   return "INVALID CART TYPE";
    }
}


const char *ramSizeName(u8 romSize) {
    switch (romSize) {
        case  0: return " 16 kBit";
        case  1: return " 64 kBit";
        case  2: return "256 kBit";
        case  3: return "  2 MBit";
        case  4: return "  1 MBit";
        default: return "INVALID RAM SIZE";
    }
}

const char *romSizeName(u8 romSize) {
    switch (romSize) {
        case  0x0: return "256 kBit";
        case  0x1: return "512 kBit";
        case  0x2: return "  1 MBit";
        case  0x3: return "  2 MBit";
        case  0x4: return "  4 MBit";
        case  0x5: return "  8 MBit";
        case  0x6: return " 16 MBit";
        case 0x52: return "  9 MBit";
        case 0x53: return " 10 MBit";
        case 0x54: return " 12 MBit";
        default: return "INVALID ROM SIZE";
    }
}

const char *destinationCodeName(u8 code) {
    switch (code) {
        case 0: return "Japanese";
        default: return "Non-Japanese";
    }
}

Cartridge::Cartridge(std::istream *romFile) {
    romFile->seekg(0, std::ios::end);
    romSize = romFile->tellg();

    if (romSize > MAX_ROM_SIZE) {
        status = FILE_LOAD_ERROR;
        std::cout << "Rom file size greater than max file size of " << romSize
            << std::endl;
    }

    romFile->seekg(0, std::ios::beg);
    rom = new u8[romSize];
    romFile->read( (char *) rom, romSize);

    if (!romFile->good()) {
        status = FILE_LOAD_ERROR;
        std::cout << "Input stream status not good, failed to load rom" <<
            std::endl;
    }

    bank0 = rom;
    bank1 = rom; // bank1 is indexed from [0x4000,0x8000)
    status = CARTRIDGE_LOADED;
}

Cartridge::~Cartridge() {
    if (ramSize)
        delete[] ram;
    if (romSize)
        delete[] rom;
    ram = nullptr;
    rom = nullptr;
    bank0 = nullptr;
    bank1 = nullptr;
}

u8 *Cartridge::getTitle() {
    return &rom[TITLE_START_ADDR];
}

bool Cartridge::isColorMode() {
    return rom[CGB_MODE_ADDR] = CGB_MODE_VALUE;
}

u8 Cartridge::getCartridgeType() {
    return rom[CART_TYPE_ADDR];
}

const char* Cartridge::getCartridgeTypeName() {
    return cartType(rom[CART_TYPE_ADDR]);
}

u32 Cartridge::getRomSize() {
    return romSize;
}

u32 Cartridge::getRamSize() {
    return ramSize;
}

u8 Cartridge::getRegionCode() {
    return rom[REGION_CODE_ADDR];
}

CartridgeStatus Cartridge::getCartridgeStatus() {
    return status;
}

u8 *Cartridge::fetch(u16 pc) {
    if (pc < BANK_SIZE)
        return &bank0[pc];
    if (pc > BANK_SIZE)
        return &bank1[pc - BANK_SIZE];
}

void Cartridge::cartRomMode(u16 addr, u8 val) {
    // do nothing for ROM ONLY games
}
