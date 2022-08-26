#include "Renderer.h"

#include <SDL_render.h>
#include <SDL_pixels.h>
#include <SDL_image.h>

#include "SDLErrorHandler.h"

Renderer::Renderer(SDL_Renderer* data)
    : m_Data(data), m_CameraPtr(nullptr) {
    setColor({ 0x00, 0x00, 0x00, 0xFF });
}

Renderer::~Renderer() {
    destroy();
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

void Renderer::setColor(const SDL_Color& Color) {
    SDLAssert(SDL_SetRenderDrawColor(m_Data, Color.r, Color.g, Color.b, Color.a) == 0);
}

void Renderer::clear() {
    SDLAssert(SDL_RenderClear(m_Data) == 0);
}

void Renderer::render(Texture& SourceTexture, const SDL_Rect& Src, const SDL_Rect& Dst) {
    const SDL_Rect FinalDst = m_CameraPtr->transform(Dst);
    SDL_RenderCopy(m_Data, SourceTexture.data(), &Src, &FinalDst);
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