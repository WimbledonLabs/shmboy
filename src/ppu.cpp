#include "ppu.h"

ChrData Ppu::getChrData(u8 objNum) {
    return ChrData{ mem->get(OAM + objNum * 4 + 0),
                    mem->get(OAM + objNum * 4 + 1),
                    mem->get(OAM + objNum * 4 + 2),
                    mem->get(OAM + objNum * 4 + 3) };
}

u32 Ppu::getPixelColor(ChrData chr, int x, int y) {
    return 0; // TODO    
}

Ppu::Ppu(Cpu *cpu, GbMemory *mem) {
    this->cpu = cpu;
    this->mem = mem;
    bank0 = &(mem->memory[DISPLAY_RAM_START]);
    bank1 = mem->displayRamBank1;
}

Ppu::~Ppu() {

}

u32 Ppu::getBGPixel(int x, int y) {
    x = (x + mem->get(SCX)) % 256;
    y = (y + mem->get(SCY)) % 256;
    u8 x_tile = x/8;
    u8 y_tile = y/8;

    return 0; // TODO
}

u32 Ppu::ordinalColor(u8 pixel, u8 palette) {
    assert (pixel < 4);
    assert (palette < 8);
    return colorPalette[palette][pixel];
}

u32 Ppu::bgTileColor(u8 chrCode, u8 tX, u8 tY, BGDisplayAttr attr) {
    u8 addr = 0;
    u8 pixel = 0;
    assert (tX < 8 && tY < 8);
    if (mode == DMG_MODE) {
        assert (attr.value == 0);
    }

    if (attr.nsFlip) {
        tY = 7 - tY;
    }
    if (attr.ewFlip) {
        tX = 7 - tX;
    }

    // Offset row by chrCode*16 + tY*2
    // 16 bytes per tile
    // 2 bytes per row
    u8 row = chrCode*16 + tY*2;

    if (chrCode < 0x80 && !(LCDC & 0x10) ) {
        // Offset the row by 0x1000 if BG tiles 0x00 through 0x7F are
        // located in 0x9000-0x97FF as defined by the LCDC register
        // (tiles are located in 0x8000 through 0x87FF otherwise)
        row += 0x1000;
    }
    
    // switch based on bank
    u8 row0Data = (attr.bank) ? bank0[row+0] : bank1[row+0];
    u8 row1Data = (attr.bank) ? bank0[row+1] : bank1[row+1];
    pixel  = (row0Data & (0x1 << tX)) >>  tX;
    pixel |= (row1Data & (0x1 << tX)) >> (tX-1);

    return ordinalColor(pixel, attr.palette);
}

/* More complex, but maybe fater implementation?
void Ppu::drawBackground() {
    u8 backgroundNumber = LCDC & 0x08 // TODO LCDC reference
    u8 bgStartOffset = (backgroundNumber) ? BG_DISPLAY_2_DATA_OS :
                                            BG_DISPLAY_1_DATA_OS;
    for (int y=0; y < BG_TILE_COUNT_V; y++) {
        for (int x=0; x < BG_TILE_COUNT_H; x++) {
            u8 chrCode = bank0[bgStartOffset + x + y*BG_TILE_COUNT_H];
            BGDisplayAttr attr;
            
            if (mode == CGB_MODE) {
                attr.value = bank1[bgStartOffset + x + y*BG_TILE_COUNT_H];
            } else {
                attr.value = 0;
            }

            for (int tY = 0; tY < 16; tY++) {
                for (int tX = 0; tX < 8; tX++) {
                    // TODO implement horizontal and vertical flipping (CGB)
                    bgWriteBuffer[x*8 + tX + (y*8+tY)*BG_DOT_WIDTH] = 
                        bgTileColor(chrCode, tX, tY, attr);
                }
            }
        }
    }
} */

void Ppu::drawBackground() {
    u8 backgroundNumber = mem->get(LCDC) & 0x08; // TODO LCDC reference
    u8 bgStartOffset = (backgroundNumber) ? BG_DISPLAY_2_DATA_OS :
                                            BG_DISPLAY_1_DATA_OS;
    for (int y=0; y < SCREEN_WIDTH; y++) {
        for (int x=0; x < SCREEN_HEIGHT; x++) {
            int tileNumber = x/8 + (y/8)*BG_TILE_COUNT_HEIGHT;
            int tX = x % 8;
            int tY = y % 8;
            u8 chrCode = bank0[bgStartOffset + tileNumber];
            BGDisplayAttr attr;
            
            if (mode == CGB_MODE) {
                attr.value = bank1[bgStartOffset + x + y*BG_TILE_COUNT_HEIGHT];
            } else {
                attr.value = 0;
            }

            bgWriteBuffer[x + y*SCREEN_WIDTH] = 
                bgTileColor(chrCode, tX, tY, attr);
        }
    }
}

u32 Ppu::getPixelData(ChrData chr, int x, int y) {
    return 0; // TODO
}

void Ppu::drawPixel(int x, int y) {
    bool objIsPresent = false;
    ChrData top; // TODO: should be the background tile
    ChrData compare;
    for (int i=0; i<MAX_OBJ_COUNT; i++) {
        compare = getChrData(i);
        if (compare.x >= x && compare.x < x+8) {
            if ( EIGHT_BY_SIXTEEN_MODE ) {
                break; // TODO not implemented
            } else {
                if (compare.y >= y && compare.y < y+8) {
                    if (mode == CGB_MODE) {
                        top = compare;
                        objIsPresent = true;
                        return;
                    } else {
                        if (compare.x < top.x) {
                            top = compare;
                            objIsPresent = true;
                        }
                    }
                }
            }
        }
    }
    writeBuffer[x + y*SCREEN_WIDTH] = getPixelData(top, x, y);
}

void Ppu::swapBuffers() {
    //u32 *tmp = readBuffer;
    //readBuffer = writeBuffer;
    //writeBuffer = tmp;

    //bufferChanged = true;
    return; // TODO
}

u32* Ppu::getPixelBuffer() {
    bufferChanged = false;
    return readBuffer;
}
