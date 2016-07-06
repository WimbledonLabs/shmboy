#include "shmboy_common.h"

class PPU {
    PPU(Core core, u8 *bank0, u8 *bank1, u8 *vbk, u8 *svbk);
    void update(float timeStep);
    u32 getPixelBuffer();
}
