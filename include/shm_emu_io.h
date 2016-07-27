#ifndef SHM_EMU_IO
#define SHM_EMU_IO

#include "shmboy_common.h"
#include <SDL2/SDL.h>

class Screen {
public:
    Screen(SDL_Renderer* ren, u32 width, u32 height);
    ~Screen();
    void updateScreen(u32* data);
    SDL_Texture *m_tex = nullptr;
};

#endif // SHM_EMU_IO
