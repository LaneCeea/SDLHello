#ifndef SDLERRORHANDLER_H_
#define SDLERRORHANDLER_H_

#include <cstdlib>
#include <cstdio>
#include <SDL_error.h>

#ifdef NDEBUG
#define SDLAssert(x)   (void)(x)
#define IMGAssert(x)   (void)(x)
#define DEBUGLOG(...)  (void)(__VA_ARGS__)

#else // ^^^ NDEBUG ^^^ // vvv !NDEBUG vvv
#define SDLAssert(x) if(!(x)) {             \
    SDLPrintError(#x, __FILE__, __LINE__);  \
    std::abort();                           \
}

#define IMGAssert(x) if(!(x)) {             \
    IMGPrintError(#x, __FILE__, __LINE__);  \
    std::abort();                           \
}

#define DEBUGLOG(...) std::printf(__VA_ARGS__)

#endif // NDEBUG

void SDLPrintError(const char* func, const char* file, int line);
void IMGPrintError(const char* func, const char* file, int line);

#endif // !SDLERRORHANDLER_H_