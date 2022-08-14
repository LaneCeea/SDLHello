#include "Renderer.h"

#include <cstdio>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include <SDL_image.h>

#include "Window.h"
#include "SDLErrorHandler.h"

Renderer::Renderer(Window& DisplayedWindow)
    : m_Data(nullptr) {
    init(DisplayedWindow);
    setColor({ 0xFF, 0xFF, 0xFF, 0xFF });
}

Renderer::~Renderer() {
    destroy();
}

void Renderer::init(Window& DisplayedWindow) {
    m_Data = SDL_CreateRenderer(
        DisplayedWindow.data(), -1, SDL_RENDERER_ACCELERATED
    );
    SDLAssert(m_Data != nullptr);
}

void Renderer::destroy() {
    SDL_DestroyRenderer(m_Data);
}

void Renderer::printInfo() {
    SDL_RendererInfo Info;
    SDL_GetRendererInfo(m_Data, &Info);
    std::printf("Current SDL_Renderer: %s\n", Info.name);
}

Texture Renderer::createTexture(const char* path) {
    SDL_Surface* LoadedSurface = IMG_Load(path);
    IMGAssert(LoadedSurface != nullptr);

    SDL_Texture* TextureData = SDL_CreateTextureFromSurface(m_Data, LoadedSurface);
    int w = LoadedSurface->w;
    int h = LoadedSurface->h;
    SDLAssert(TextureData != nullptr);
    
    SDL_FreeSurface(LoadedSurface);
    return Texture(TextureData, w, h);
}

void Renderer::setAlphaBlending() {
    SDLAssert(SDL_SetRenderDrawBlendMode(m_Data, SDL_BLENDMODE_BLEND) == 0);
}

void Renderer::setColor(SDL_Color Color) {
    SDLAssert(SDL_SetRenderDrawColor(m_Data, Color.r, Color.g, Color.b, Color.a) == 0);
}

void Renderer::clear() {
    SDLAssert(SDL_RenderClear(m_Data) == 0);
}

void Renderer::render(Texture& SourceTexture, const SDL_Rect& Src, const SDL_Rect& Dst) {
    SDL_RenderCopy(m_Data, SourceTexture.data(), &Src, &Dst);
}

void Renderer::render(Texture& SourceTexture, const std::vector<LabeledRect>& Srcs, const std::vector<LabeledRect>& Dsts) {
    for (const auto& Dst : Dsts) {
        render(SourceTexture, Srcs[Dst.SrcID].Rect, Dst.Rect);
    }
}

void Renderer::render(Texture& SourceTexture) {
    render(SourceTexture, SourceTexture.getSrc(), SourceTexture.getDst());
}


void Renderer::present() {
    SDL_RenderPresent(m_Data);
}