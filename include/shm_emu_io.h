#ifndef SHM_EMU_IO
#define SHM_EMU_IO

#include "shmip8_common.h"
#include <SDL2/SDL.h>

class Screen {
public:
    Screen(SDL_Renderer* ren, uint32 width, uint32 height);
    ~Screen();
    void updateScreen(uint32* data);
    SDL_Texture *m_tex = nullptr;
};

#endif // SHM_EMU_IO
