#include "SDLErrorHandler.h"

#include <iostream>
#include <SDL_error.h>

void SDLPrintError() {
    std::cout << "[SDL Error] (" << SDL_GetError() << ")\n";
    SDL_ClearError();
}

void IMGPrintError() {
    std::cout << "[SDL_image Error] (" << SDL_GetError() << ")\n";
    SDL_ClearError();
}