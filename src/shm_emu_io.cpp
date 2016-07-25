#include "shm_emu_io.h"

Screen::Screen(SDL_Renderer* ren, uint32 width, uint32 height) {
    // Create a texture to hold the pixel data while rendering
    m_tex = SDL_CreateTexture(ren,
                            SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING,
                            width, height);
    uint32 data[width*height] = {0};
    updateScreen(data);
}

Screen::~Screen() {
    SDL_DestroyTexture(m_tex);
    m_tex = nullptr;
}

void Screen::updateScreen(uint32* data) {
    int w;
    int h;
    SDL_QueryTexture(m_tex, NULL, NULL, &w, &h);

    const void *pix = data;

    SDL_UpdateTexture(m_tex, NULL, pix, w * sizeof (Uint32));
}
