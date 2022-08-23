#ifndef SDLERRORHANDLER_H_
#define SDLERRORHANDLER_H_

#include <cstdlib>
#include <cstdio>
#include <SDL_error.h>

#ifdef NDEBUG
#define SDLAssert(x) (void)(x)
#define IMGAssert(x) (void)(x)
#define DEBUGLOG(x)  (void)(x)

#else // ^^^ NDEBUG ^^^ // vvv !NDEBUG vvv
#define SDLAssert(x) if(!(x)) {                                                         \
    std::printf("[SDL Error] (%s): %s %s:%d\n", SDL_GetError(), #x, __FILE__, __LINE__);\
    std::abort();                                                                       \
}

#define IMGAssert(x) if(!(x)) {                                                                 \
    std::printf("[SDL_image Error] (%s): %s %s:%d\n", SDL_GetError(), #x, __FILE__, __LINE__);  \
    std::abort();                                                                               \
}

#define DEBUGLOG(x) std::printf(x)

#endif // NDEBUG

void SDLPrintError();
void IMGPrintError();

#endif // !SDLERRORHANDLER_H_