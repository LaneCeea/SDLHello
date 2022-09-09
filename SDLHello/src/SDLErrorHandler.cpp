#include "SDLErrorHandler.h"

#include <cstdio>
#include <SDL_error.h>

void SDLPrintError(const char* func, const char* file, int line) {
    std::printf("[SDL Error] (%s): %s %s:%d\n", SDL_GetError(), func, file, line);
    SDL_ClearError();
}

void IMGPrintError(const char* func, const char* file, int line) {
    std::printf("[SDL_image Error] (%s): %s %s:%d\n", SDL_GetError(), func, file, line);
    SDL_ClearError();
}