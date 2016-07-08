ChrData PPU::getChrData(u8 objNum) {
    return ChrData( OAM[objNum * 4 + 0],
                    OAM[objNum * 4 + 1],
                    OAM[objNum * 4 + 2],
                    OAM[objNum * 4 + 3] )
}

u32 PPU::getPixelColor(ChrData chr, int x, int y) {
    
}

PPU::PPU() {

}

u32 PPU::getBGPixel(int x, int y) {
    x = (x + displayRam[SCX_OS]) % 256;
    y = (y + displayRam[SCY_OS]) % 256;
    x_tile = x/8;
    y_tile = y/8;
}

void PPU::drawBackground() {
    u8 backgroundNumber = LCDC & 0x08 // TODO LCDC reference
    u8 bgStartOffset = (backgroundNumber) ? BG_DISPLAY_2_DATA_OS :
                                            BG_DISPLAY_1_DATA_OS;
    for (int y=0; y < BG_TILE_HEIGHT; y++) {
        for (int x=0; x < BG_TILE_WIDTH; x++) {
            u8 chrCode = bank0[bgStartOffset + x + y*BG_TILE_WIDTH];
            u8 attr    = bank1[bgStartOffset + x + y*BG_TILE_WIDTH];
            int startX = 0;
            int stopX = startX + 8;
            int stepX = 1;
            int startY = 0;
            int stopY = 0;
            int stepY = 1;

            for (int tY = startY; tY < stopY; tY += stepY) {
                for (int tX = startX; tX < stopX; tX += stepX) {
                    
                }
            }
        }
    }
}

void PPU::drawPixel(int x, int y) {
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

void *PPU::swapBuffers() {
    u32 *tmp = readBuffer;
    readBuffer = writeBuffer;
    writeBuffer = tmp;

    bufferChanged = true;
}

u32* PPU::getPixelBuffer() {
    bufferChanged = false;
    return readBuffer;
}
