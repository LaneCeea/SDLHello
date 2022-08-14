#include "Texture.h"

#include <SDL_render.h>

#include "SDLErrorHandler.h"

Texture::Texture(SDL_Texture* data, int w, int h)
    : m_Data(data), m_Width(w), m_Height(h) {
    m_Src.emplace_back(0, 0, 0, w, h);
    m_Dst.emplace_back(0, 0, 0, w, h);
}

Texture::~Texture() {
    destroy();
}

void Texture::destroy() {
    SDL_DestroyTexture(m_Data);
}