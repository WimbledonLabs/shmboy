#include "shmboy_common.h"

// DMG -> monochrome gameboy
// CGB -> color game boy

// DMG/CGB LCD display registers occupy memory locations FF40 to FF4B
// The following are the offset from FF40 fo each of the LCD registers
#define LCDC_OS 0
#define STAT_OS 1
#define SCY_OS  2
#define SCX_OS  3
#define LY_OS   4
#define LYC_OS  5
#define DMA_OS  6
#define BGP_OS  7
#define OBP0_OS 8
#define OBP1_OS 9
#define WY_OS 0xA
#define WX_OS 0xB

// CGB LCD register offsets from FF51
#define HDMA1_OS 0x1
#define HDMA2_OS 0x2
#define HDMA3_OS 0x3
#define HDMA4_OS 0x4
#define HDMA5_OS 0x5
#define BCPS_OS  0x18
#define BCPD_OS  0x19
#define OCPS_OS  0x1A
#define OCPD_OS  0x1B

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144
#define BG_WIDTH 255
#define BG_HEIGHT 255
#define BG_TILE_WIDTH 32
#define BG_TILE_HEIGHT 32
#define MAX_OBJ_COUNT 40

#define EIGHT_BY_SIXTEEN_MODE 0 // TODO implement

#define OBJ_CHR_DATA_OS 0x0000
#define BG_CHR_DATA_OS  0x1000
#define BG_DISPLAY_1_DATA_OS 0x1800
#define BG_DISPLAY_2_DATA_OS 0x1C00

typedef struct {
    u8 x;
    u8 y;
    u8 code;
    u8 attr;
} ChrData;

class PPU {
    PPU(Core core,
        u8 *OAM,   //
        u8 *bank0, // 0x8000 bank 0
        u8 *bank1, // 0x8000 bank 1
        u8 *vbk,   // ptr to bank select register
        u8 *svbk,  // ptr to thr other bank select register?
        bool isCgb);
    void update(int clockCycles);
    u32 *getPixelBuffer();

    void swapBuffers();

    u32 *bgWriteBuffer;

    u32 *writeBuffer;
    u32 *readBuffer;

    bool hBlank;
    bool vBlank;

    bool bufferChanged;

    GbMode mode;
}
