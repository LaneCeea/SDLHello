#include "SDLHandler.h"

#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

#include "SDLErrorHandler.h"

namespace SDL {

void init() {
    SDLAssert(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0);
    IMGAssert(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
}

void printVersion() {
    SDL_version version;
    SDL_VERSION(&version);
    std::printf("SDL version: %u.%u.%u\n",
        (uint32_t)version.major, (uint32_t)version.minor, (uint32_t)version.patch);
    SDL_IMAGE_VERSION(&version);
    std::printf("SDL_image version: %u.%u.%u\n",
        (uint32_t)version.major, (uint32_t)version.minor, (uint32_t)version.patch);
}

void quit() {
    IMG_Quit();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

}