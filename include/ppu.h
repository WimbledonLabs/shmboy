#ifndef PPU_H
#define PPU_H

#include "shmboy_common.h"
#include "gb_registers.h"
#include "cpu.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define BG_DOT_WIDTH 255
#define BG_DOT_HEIGHT 255
#define BG_TILE_COUNT_WIDTH 32
#define BG_TILE_COUNT_HEIGHT 32
#define MAX_OBJ_COUNT 40

#define NUM_PALETTES 8
#define COLORS_PER_PALETTE 4

#define EIGHT_BY_SIXTEEN_MODE 0 // TODO implement

// Forward declarations
class Cpu;

typedef union {
    u8 value;
    struct {
        unsigned int palette: 3;
        unsigned int bank: 1;
        unsigned int: 1;
        unsigned int ewFlip: 1;
        unsigned int nsFlip: 1;
        unsigned int priority: 1;
    };
} BGDisplayAttr;

typedef struct {
    u8 x;
    u8 y;
    u8 code;
    u8 att;
} ChrData;

class Ppu {
public:
    Ppu(Cpu      *cpu,
        Mmu *mem);

    ~Ppu();

    Cpu *cpu;
    Mmu *mem;

    u8 *bank0;
    u8 *bank1;

    void setMode(GbMode);

    void update(int clockCycles);
    u32 *getPixelBuffer();

    u32 bgTileColor(u8 chrCode, u8 tX, u8 tY, BGDisplayAttr attr);
    void swapBuffers();

    u32 colorPalette[NUM_PALETTES][COLORS_PER_PALETTE];

    u32 bgWriteBuffer[BG_DOT_WIDTH * BG_DOT_HEIGHT];
    u32 writeBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    u32 readBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    bool hBlank;
    bool vBlank;

    bool bufferChanged;

    GbMode mode;

    void blit(void *src, void *dest, 
               int srcx, int srcy,
               int destx, int desty);

    void drawBgLayer(u32 *layer, bool window);
    void drawObjLayer(u32 *layer);




u32 getPixelData(ChrData chr, int x, int y);

void drawPixel(int x, int y);
void drawBackground();

u32 ordinalColor(u8 pixel, u8 palette);
ChrData getChrData(u8 objNum);
u32 getBGPixel(int x, int y);
u32 getPixelColor(ChrData chr, int x, int y);



};

#endif // PPU_H
